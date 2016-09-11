
CC=m68k-amigaos-gcc

CFLAGS = -s -noixemul -Os -lmpfr -lgmp -lm

acalc: acalc.c
	$(CC) -o acalc acalc.c $(CFLAGS)
	ls -l acalc

clean:
	rm acalc
