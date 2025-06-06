CC := clang
CFLAGS := -g $(shell pkg-config sdl2 SDL2_image SDL2_ttf --cflags)
LDFLAGS := $(shell pkg-config sdl2 SDL2_image SDL2_ttf --libs)

SRCS := main.c astronaut.c object.c backgrounds.c alien.c util.c
HDRS := astronaut.h object.h backgrounds.h alien.h util.h

OBJS := $(SRCS:.c=.o)

EXEC := game

# default recipe
all: $(EXEC)

# recipe for building the final executable
$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)

.PHONY: all clean
