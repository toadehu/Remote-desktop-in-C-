cmake_minimum_required(VERSION 3.10)
project(GpuStreamingSystem C CXX)

# --- 1. Options and Standards ---
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON) # Set to ON for _GNU_SOURCE features like memfd_create

option(USE_CUDA "Enable HIP for NVIDIA (CUDA) backend" OFF)

# --- 2. Platform & Toolchain Detection ---
if(WIN32 OR MINGW)
  add_definitions(-DWIN -DmainCRTStartup)
  set(PLATFORM_LIBS mmingw32 SDL2main)
elseif(APPLE)
  # MacOS specific, not supported
else()
  # Linux-specific dependencies
  find_package(PkgConfig REQUIRED)

  # Core Graphics and Wayland Stack
  pkg_check_modules(WAYLAND REQUIRED wayland-client wayland-egl)
  pkg_check_modules(GBM REQUIRED gbm)
  pkg_check_modules(EGL REQUIRED egl)
  pkg_check_modules(DRM REQUIRED libdrm)
  pkg_check_modules(PORTAL REQUIRED libportal)
  pkg_check_modules(GLIB REQUIRED glib-2.0 gio-2.0)

  find_program(WAYLAND_SCANNER NAMES wayland-scanner REQUIRED)

  # X11 or Wayland Detection
  if(DEFINED ENV{WAYLAND_DISPLAY})
    add_definitions(-DHAVE_WAYLAND)
    message(STATUS "Detected Wayland environment")

    # --- Wayland Protocol Generation ---
    # Helper to generate headers/code from Wayland XMLs
    function(wayland_generate_protocol xml_path out_sources)
      get_filename_component(basename ${xml_path} NAME_WE)
      set(header "${CMAKE_CURRENT_BINARY_DIR}/${basename}.h")
      set(code "${CMAKE_CURRENT_BINARY_DIR}/${basename}.c")

      add_custom_command(
                OUTPUT "${header}"
                COMMAND ${WAYLAND_SCANNER} client-header "${xml_path}" "${header}"
                DEPENDS "${xml_path}"
            )
      add_custom_command(
                OUTPUT "${code}"
                COMMAND ${WAYLAND_SCANNER} private-code "${xml_path}" "${code}"
                DEPENDS "${xml_path}"
            )
      set(${out_sources} ${${out_sources}} "${header}" "${code}" PARENT_SCOPE)
    endfunction()

    # Define paths to protocols (adjust these paths if your distro stores them elsewhere)
    # Usually /usr/share/wayland-protocols/
    set(WL_PROTO_DIR "/usr/share/wayland-protocols")

    wayland_generate_protocol("${WL_PROTO_DIR}/unstable/linux-dmabuf/linux-dmabuf-unstable-v1.xml" WAYLAND_PROTO_SRCS)
    # Note: wlr-screencopy is often in /usr/share/wlr-protocols/ or needs to be in your local src
    wayland_generate_protocol("${CMAKE_CURRENT_SOURCE_DIR}/protocols/wlr-screencopy-unstable-v1.xml" WAYLAND_PROTO_SRCS)

  else()
    add_definitions(-DHAVE_X11)
    set(X11_REQUIRED_LIBS X11 Xinerama Xext)
    message(STATUS "Detected X11 environment")
  endif()
endif()

# --- 3. Dependency Discovery ---
find_package(SDL2 REQUIRED)
find_package(OpenCL)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# --- 4. HIP / Backend Sanity Checks ---
set(BACKEND_DEFINE "-DBACKEND_OPENCL")

if(USE_CUDA)
  execute_process(COMMAND nvidia-smi OUTPUT_VARIABLE NVIDIA_GPU_FOUND ERROR_QUIET)
  if(NVIDIA_GPU_FOUND)
    message(STATUS "NVIDIA GPU Detected")
    find_package(HIP REQUIRED)
    set(BACKEND_DEFINE "-DBACKEND_HIP_NVIDIA")
  else()
    message(FATAL_ERROR "CUDA backend requested but no NVIDIA GPU found!")
  endif()
else()
  find_package(HIP QUIET)
  if(HIP_FOUND)
    set(BACKEND_DEFINE "-DBACKEND_HIP_AMD")
  else()
    message(STATUS "HIP not found: Falling back to OpenCL")
  endif()
endif()

add_definitions(${BACKEND_DEFINE} -DCL_TARGET_OPENCL_VERSION=300)

# --- 5. Global Flags ---
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-ggdb3 -Wall -msse2 -march=native -D_GNU_SOURCE)
  if(NOT MINGW)
    add_compile_options(-mavx2)
  endif()
endif()

# --- 6. Source Groups ---
set(COMMON_SOURCES
    src/opencl_wrap.c
    src/dummy_test.c
    ${WAYLAND_PROTO_SRCS} # Include generated protocol code
)

# Crucial: Include the binary directory so generated headers can be found
include_directories(
    headers
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${WAYLAND_INCLUDE_DIRS}
    ${GBM_INCLUDE_DIRS}
    ${EGL_INCLUDE_DIRS}
    ${DRM_INCLUDE_DIRS}
    ${PORTAL_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS}
)

# --- 7. Executable Targets ---

# Server (Needs Wayland + GBM + EGL for zero-copy)
add_executable(streaming_server src/server_main.c ${COMMON_SOURCES})
target_link_libraries(streaming_server
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${OpenCL_LIBRARIES}
    ${WAYLAND_LIBRARIES}
    ${GBM_LIBRARIES}
    ${EGL_LIBRARIES}
    ${DRM_LIBRARIES}
    ${X11_REQUIRED_LIBS}
    ${PLATFORM_LIBS}
    ${PORTAL_INCLUDE_LIBS}
    ${GLIB_INCLUDE_LIBS}
    m
)

# Client
add_executable(streaming_client src/client_main.c ${COMMON_SOURCES})
target_link_libraries(streaming_client
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${OpenCL_LIBRARIES}
    ${WAYLAND_LIBRARIES}
    ${X11_REQUIRED_LIBS}
    ${PLATFORM_LIBS}
    ${PORTAL_INCLUDE_LIBS}
    ${GLIB_INCLUDE_LIBS}
    m
)
