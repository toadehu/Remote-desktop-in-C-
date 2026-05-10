# --- Compiler and Global Flags ---
CC      := gcc
# Added -Iheaders so you don't have to use relative paths in your #include statements
CFLAGS  := $(shell pkg-config --cflags sdl2 SDL2_image) -Iheaders -ggdb3 -std=c11 -Wall -msse2 -mavx2 -march=native
LDLIBS  := $(shell pkg-config --libs sdl2 SDL2_image --libs wayland-client) -lm -lSDL2_ttf -lx264 -lxcb -lxcb-image -lX11 -lavcodec -lavutil -lavformat -lOpenCL

# --- Build Logic ---
ifeq ($(version),DEBUG)
    CFLAGS += -D_DEBUG=1 -g
else
    CFLAGS += -O3
endif

# Windowing System Detection
# 1. Check for Wayland first (Native)
ifeq ($(shell test -n "$$WAYLAND_DISPLAY" && echo yes),yes)
    CFLAGS  += -DHAVE_WAYLAND
    # Add Wayland libraries if needed, e.g., -lwayland-client
    $(info Compiling for Wayland)

# 2. Check for X11 second (Legacy/XWayland)
else ifeq ($(shell test -n "$$DISPLAY" && echo yes),yes)
    CFLAGS  += -D__HAVE_X11__
    LDLIBS  += -lX11
    $(info Compiling for X11)

# Fallback error
else
    $(error Unsupported windowing system. Neither DISPLAY nor WAYLAND_DISPLAY is set)
endif

# --- Files and Targets ---
# Headers (Used for dependency tracking)
HDRS := $(wildcard headers/*.h)

# Client Files
CLIENT_SRCS := src/ClientApp/main.c src/opencl_wrap.c
CLIENT_OBJS := $(CLIENT_SRCS:.c=.o)
CLIENT_EXEC := client_app

# Server Files
SERVER_SRCS := src/server/main.c src/opencl_wrap.c
SERVER_OBJS := $(SERVER_SRCS:.c=.o)
SERVER_EXEC := server_app

# --- Recipes ---

all: $(CLIENT_EXEC) $(SERVER_EXEC)

# Build Client
$(CLIENT_EXEC): $(CLIENT_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

# Build Server
$(SERVER_EXEC): $(SERVER_OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

# Generic Rule for Object Files
# This handles the .c to .o conversion for any file, tracking header changes
%.o: %.c $(HDRS) Makefile
	$(CC) -c $(CFLAGS) $< -o $@

# --- Utility Commands ---

clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC) $(CLIENT_OBJS) $(SERVER_OBJS)

# Debug Run Examples
run_client: $(CLIENT_EXEC)
	./$(CLIENT_EXEC)

run_server: $(SERVER_EXEC)
	./$(SERVER_EXEC)

.PHONY: all clean run_client run_server
