# Compiler settings
ifeq ($(version),RASPI)
	CC := arm-none-eabi-gcc
else ifeq ($(version),WIN)
	CC := x86_64-w64-mingw32-gcc
else
	CC := gcc
endif

# Compiler flags
CFLAGS := -ggdb3 -std=c89 -Wall -I../../headers

# Platform-specific settings
ifeq ($(version),RASPI)
	SDL_DIR := $(HOME)/SDL2-2.26.5
	CFLAGS += -I$(SDL_DIR)/include -march=armv8-a -mfloat-abi=hard -mfpu=neon-fp-armv8 -mtune=cortex-a53 --short-enums -fshort-enums
	LDFLAGS += -L$(SDL_DIR)/lib -Wl,-Bstatic -lSDL2 -Wl,-Bdynamic
else
	CFLAGS += $(shell pkg-config --cflags sdl2 SDL2_image)
	LDFLAGS += $(shell pkg-config --libs sdl2 SDL2_image)
endif

# Additional flags for Windows and debug configurations
ifeq ($(version),WIN)
	CFLAGS += -DmainCRTStartup
	LDFLAGS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
else ifeq ($(version),DEBUG)
	CFLAGS += -D_DEBUG=1 -g -msse2 -mavx2 -march=native -DHAVE_X86
else ifndef version
	CFLAGS += -O3 -msse2 -mavx2 -march=native -DHAVE_X86
endif

# Fast compilation settings
ifeq ($(version),FAST)
	CFLAGS += -flto
endif

# Determine windowing system
ifeq ($(shell test -n "$${DISPLAY+x}" && echo yes),yes)
    CFLAGS+=-DHAVE_X11
	CFLAGS+=-lX11 -lXext
	LDLIBS+=-lX11 -lXext
else ifeq ($(shell test -n "$${WAYLAND_DISPLAY+x}" && echo yes),yes)
    CFLAGS+=-DHAVE_WAYLAND
else
    $(error "Unsupported windowing system")
endif

# Linker flags
LDFLAGS += -lm -lSDL2_ttf -lX11 -msse2 -mavx2 -march=native

# Source and object files
SRCS := ./main.c
OBJS := $(SRCS:.c=.o)

# Executable name
EXEC := casa_manelelor

# Default target
all: $(EXEC)
	@echo "Build version: $(version)"

# Help target
help:
	@echo "Usage: make [target] [version=VERSION]"
	@echo "Targets:"
	@echo "  all       : Build the executable."
	@echo "  showfont  : Build the showfont utility."
	@echo "  glfont    : Build the glfont utility."
	@echo "  test      : Clean and rebuild for debugging."
	@echo "  new       : Clean and rebuild."
	@echo "  run       : Build and run the executable."
	@echo "  clean     : Remove build artifacts."
	@echo "VERSION can be one of: RASPI, WIN, DEBUG, or not set (default build)."

# Utility targets
showfont: showfont.c Makefile
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

glfont: glfont.c Makefile
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

# Building the executable
$(EXEC): $(OBJS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

# Compiling object files
$(OBJS): %.o: %.c Makefile
	$(CC) -c $(CFLAGS) $< -o $@

# Cleaning up
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean help test new run

