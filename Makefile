SOURCES = utils.c prettyprint.c pool.c character.c rooms.c items.c adventure.c

final:
	gcc utils.c prettyprint.c pool.c character.c rooms.c items.c adventure.c -o clue

debug:
	gcc -g -DDEBUG=1 $(SOURCES) -o clue

test:
	gcc -g -DDEBUG=1 -DTEST=1 $(SOURCES) -o clue

# COPYPASTE: gcc utils.c prettyprint.c pool.c character.c rooms.c items.c adventure.c