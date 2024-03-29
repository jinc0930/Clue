.PHONY: all graphics test

CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces
OBJS = utils.c character.c rooms.c items.c game.c
 
ifeq ($(disable_future),1)
    CFLAGS += -DDISABLE_FUTURE
endif

# Default target entry
all:
	gcc -o clue $(OBJS) prettyprint.c adventure.c $(CFLAGS)

graphics:
	$(MAKE) -C graphics/

# Custom test suit
test:
	gcc -o test $(OBJS) test.c $(CFLAGS)