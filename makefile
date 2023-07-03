all:
	gcc engine.c init.c bitboards.c board.c data.c hashkeys.c check.c attack.c io.c movegen.c validate.c makemove.c -o engine