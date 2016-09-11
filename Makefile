GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)
ARC=/usr/local/amiga/bin/lha
CC=m68k-amigaos-gcc
CFLAGS = -s -noixemul -Os -lmpfr -lgmp -lm -DGITVERSION=\"$(GIT_VERSION)\"


acalc: acalc.c
	$(CC) -o acalc acalc.c $(CFLAGS)
	ls -l acalc

release: acalc
	$(ARC) a acalc-$(GIT_VERSION).lha acalc

clean:
	rm -f acalc acalc*.lha
