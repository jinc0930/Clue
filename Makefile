.PHONY: all graphics test

CFLAGS = -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces
SOURCE_BASE = utils.c pool.c character.c rooms.c items.c game.c

# Default target entry
all:
	gcc -o clue $(SOURCE_BASE) prettyprint.c main.c $(CFLAGS)

graphics:
	$(MAKE) -C graphics/

# Custom test suit (hard coded)
test:
	gcc -o test $(SOURCE_BASE) tests/test.c $(CFLAGS)