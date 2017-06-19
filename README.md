
A small gui calculator for Commodore Amiga (OS>=2.0)


# Additional libraries #

For higher accuracy the GMP and MPFR libraries come in handy.  Do the configuration as shown below, but without the '-m68020' at the end. Make, install. Make clean. Reconfigure and this time add the '-m68020'. DO NOT install.  Copy manually the lib adding the 020 to the name (e.g. cp .libs/libgmp.a /usr/local/amiga/m68k-amigaos/lib/libgmp020.a and cp src/.libs/libmpfr.a /usr/local/amiga/m68k-amigaos/lib/libmpfr020.a)

# Gmp-6.1.0 #

./configure --host=x86_64-pc-linux-gnu CC=m68k-amigaos-gcc NM=m68k-amigaos-nm --prefix=/usr/local/amiga/m68k-amigaos/ --enable-assembly=no --enable-shared=no AR=m68k-amigaos-ar RANLIB=m68k-amigaos-ranlib --program-suffix=020 CFLAGS="-noixemul -fomit-frame-pointer -O3"

make

sudo ln -s /usr/local/amiga/bin/m68k-amigaos-ranlib /usr/bin/m68k-amigaos-ranlib

sudo make install

# Mpfr-3.1.4 #

./configure --host=x86_64-pc-linux-gnu CC=m68k-amigaos-gcc NM=m68k-amigaos-nm --prefix=/home/alex/amiga/gcc6/m68k-amigaos/ --enable-assembly=no --enable-shared=no AR=m68k-amigaos-ar RANLIB=m68k-amigaos-ranlib CFLAGS="-noixemul -fomit-frame-pointer -O3 -D_MPFR_H_HAVE_FILE -DMPFR_LCONV_DPTS=0"

make

sudo make install
