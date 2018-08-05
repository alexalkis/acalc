GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)
ARC=/usr/local/amiga/bin/lha
CC=m68k-amigaos-gcc

XTRALIBS = -lmpfr -lgmp -DUSEMPFR -I/home/alex/foo/include -L/home/alex/foo/lib
XTRALIBS020 = -lmpfr020 -lgmp020 -DUSEMPFR -I/home/alex/foo/include -L/home/alex/foo/lib

CFLAGS = -s -noixemul -Os -lm -DGITVERSION=\"$(GIT_VERSION)\"
CFLAGS020 = $(CFLAGS) -m68020
CFLAGSM = $(CFLAGS) $(XTRALIBS)
CFLAGSM020 = $(CFLAGS020) $(XTRALIBS020)

all: acalc acalc020 acalcm acalcm020

acalc: acalc.c
	$(CC) -o acalc acalc.c $(CFLAGS)
	ls -l acalc

acalc020: acalc.c
	$(CC) -o acalc020 acalc.c $(CFLAGS020)
	ls -l acalc020

acalcm: acalc.c
	$(CC) -o acalcm acalc.c $(CFLAGSM)
	ls -l acalcm

acalcm020: acalc.c
	$(CC) -o acalcm020 acalc.c $(CFLAGSM020)
	ls -l acalcm020


release: acalc
	$(ARC) a -o6 acalc-$(GIT_VERSION).lha acalc acalc020 acalcm acalcm020
	ls -l acalc-$(GIT_VERSION).lha

clean:
	rm -f acalc acalc020 acalcm acalcm020 acalc*.lha
