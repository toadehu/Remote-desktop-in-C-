#set the compiler 	
ifeq ($(version),RASPI)
	CC := arm-none-eabi-gcc
else ifeq ($(version),WIN)
	CC := x86_64-w64-mingw32-gcc
else
	CC := gcc
endif

#set the compiler flags
CFLAGS := -ggdb3 -std=c11 -Wall -I./hdr


ifeq ($(version),RASPI)
	SDL_DIR := $(HOME)/SDL2-2.26.5
	CFLAGS += -I$(SDL_DIR)/include -march=armv8-a -mfloat-abi=hard -mfpu=neon-fp-armv8 -mtune=cortex-a53 --short-enums -fshort-enums
	LDFLAGS += -L$(SDL_DIR)/lib -Wl,-Bstatic -lSDL2 -Wl,-Bdynamic
else
	CFLAGS += $(shell pkg-config --cflags sdl2 SDL2_image)
	LDFLAGS += $(shell pkg-config --libs sdl2 SDL2_image)
endif
	
ifeq ($(version),WIN)
	CFLAGS += -DmainCRTStartup
	LDFLAGS += -lmingw32 -lSDL2main -lSDL2 -lSDL2_image
else ifeq ($(version),DEBUG)
	CFLAGS += -D_DEBUG=1 -g -msse2 -mavx2 -march=native -DHAVE_X86
else ifndef version
	CFLAGS += -O3 -msse2 -mavx2 -march=native -DHAVE_X86
endif

ifeq ($(shell test -n "$${DISPLAY+x}" && echo yes),yes)
    # Running on X
    CFLAGS+=-DHAVE_X11
	CFLAGS+=-lX11
	LDLIBS+=-lX11
else ifeq ($(shell test -n "$${WAYLAND_DISPLAY+x}" && echo yeLDFLAGS += $(shell pkg-config --libs sdl2 SDL2_image)s),yes)
    # Running on Wayland
    CFLAGS+=-DHAVE_WAYLAND
else
    $(error "Unsupported windowing system")
endif

#set the linker flags
LDFLAGS := $(shell pkg-config --libs sdl2 SDL2_image --libs wayland-client) -lm -lSDL2_ttf -lx264 -lxcb -lxcb-image -lX11 -msse2 -mavx2 -march=native -lavcodec -lavutil -lavformat

#set ldlibs
#LDLIBS += -lyuv

#add header files here
#HDRS := framework.h

#add source files here
SRCS := main.c

#generate names of object files
OBJS := $(SRCS:.c=.o)

#name of executable
EXEC := casa_manelelor

#default recipe
all: $(EXEC)
	@echo $(version)

showfont: showfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LDFLAGS)

glfont: glfont.c Makefile
	$(CC) -o $@ $@.c $(CFLAGS) $(LDFLAGS)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

#recipe for building the final object files
$(OBJS): %.o: %.c $(HDRS) Makefile
	$(CC) -c $(CFLAGS) $< -o $@


test: clean
	$(MAKE) all version=DEBUG
	./casa_manelelor

new: clean
	$(MAKE) all
	./casa_manelelor

run: $(MAKE) all
	./casa_manelelor

#recipe to clean the workspace
clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
