# Set the compiler based on version
ifeq ($(version),RASPI)
	CC := arm-none-eabi-gcc
else ifeq ($(version),WIN)
	CC := x86_64-w64-mingw32-gcc
else
	CC := gcc
endif

# Set the compiler flags
CFLAGS := -ggdb3 -std=c89 -Wall -I../../headers

# SDL configuration for Raspberry Pi
ifeq ($(version),RASPI)
	SDL_DIR := $(HOME)/SDL2-2.26.5
	CFLAGS += -I$(SDL_DIR)/include -march=armv8-a -mfloat-abi=hard -mfpu=neon-fp-armv8 -mtune=cortex-a53 --short-enums -fshort-enums
	LDFLAGS += -L$(SDL_DIR)/lib -Wl,-Bstatic -lSDL2 -Wl,-Bdynamic
else
	# Default SDL configuration
	CFLAGS += $(shell pkg-config --cflags sdl2 SDL2_image)
	LDFLAGS += $(shell pkg-config --libs sdl2 SDL2_image)
endif

# Windows-specific flags
ifeq ($(version),WIN)
	CFLAGS += -DmainCRTStartup
	LDFLAGS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
endif

# Debug configuration
ifeq ($(version),DEBUG)
	CFLAGS += -D_DEBUG=1 -g -msse2 -mavx2 -march=native -DHAVE_X86
else ifndef version
	CFLAGS += -O3 -msse2 -mavx2 -march=native -DHAVE_X86
endif

# X11 or Wayland display server detection
ifeq ($(shell test -n "$${DISPLAY+x}" && echo yes),yes)
    # Running on X
    CFLAGS += -DHAVE_X11 -lX11
    LDLIBS += -lX11
else ifeq ($(shell test -n "$${WAYLAND_DISPLAY+x}" && echo yes),yes)
    # Running on Wayland
    CFLAGS += -DHAVE_WAYLAND
else
    $(error "Unsupported windowing system")
endif

# Set the linker flags
LDFLAGS += -lm -lSDL2_ttf -lX11 -msse2 -mavx2 -march=native

# Source and object files
SRCS := main.c
OBJS := $(SRCS:.c=.o)

# Name of executable
EXEC := casa_manelelor

# Default recipe
all: $(EXEC)
	@echo "Build version: $(version)"

# Help target
help:
	@echo "Usage: make [target] [version=VERSION]"
	@echo "Targets:"
	@echo "  all       Build the default target"
	@echo "  clean     Remove built files"
	@echo "  test      Build and run in DEBUG mode"
	@echo "  new       Clean build and run"
	@echo "  run       Build and run"
	@echo "  showfont  Build showfont utility"
	@echo "  glfont    Build glfont utility"
	@echo "Set 'version' to one of: RASPI, WIN, DEBUG for specific configurations."

# Recipes for utilities
showfont: showfont.c Makefile
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

glfont: glfont.c Makefile
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

# Recipe for building the final executable
$(EXEC): $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

# Recipe for building object files
$(OBJS): %.o: %.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

# Cleaning up
clean:
	rm -f $(EXEC) $(OBJS)

# Special targets
.PHONY: all clean help test new run showfont glfont
