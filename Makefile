#set the compiler
CC := gcc

#set the compiler flags
CFLAGS := $(shell pkg-config --cflags sdl2 SDL2_image) -ggdb3 -std=c11 -Wall -msse2 -mavx2 -march=native

ifeq ($(version),DEBUG)
	CFLAGS += -D_DEBUG=1 -g
else 
	CFLAGS += -O3
endif

ifeq ($(shell test -n "$${DISPLAY+x}" && echo yes),yes)
    # Running on X
    CFLAGS+=-DHAVE_X11
	CFLAGS+=-lX11
	LDLIBS+=-lX11
else ifeq ($(shell test -n "$${WAYLAND_DISPLAY+x}" && echo yes),yes)
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
HDRS := framework.h

#add source files here
SRCS := main.c

#generate names of object files
OBJS := $(SRCS:.c=.o)

#name of executable
EXEC := casa_manelelor

#default recipe
all: $(EXEC)

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
