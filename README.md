
A small gui calculator for Commodore Amiga (OS>=2.0)


# Gmp-6.1.0 #

`./configure --host=x86_64-pc-linux-gnu CC=m68k-amigaos-gcc NM=m68k-amigaos-nm --prefix=/usr/local/amiga/m68k-amigaos/ --enable-assembly=no --enable-shared=no AR=m68k-amigaos-ar RANLIB=m68k-amigaos-ranlib --program-suffix=020 CFLAGS="-fomit-frame-pointer -m68020 -O3"

make

sudo ln -s /usr/local/amiga/bin/m68k-amigaos-ranlib /usr/bin/m68k-amigaos-ranlib
sudo make install`


# Mpfr-3.1.4 #

`./configure --host=x86_64-pc-linux-gnu CC=m68k-amigaos-gcc NM=m68k-amigaos-nm --prefix=/usr/local/amiga/m68k-amigaos/ --enable-assembly=no --enable-shared=no AR=m68k-amigaos-ar RANLIB=m68k-amigaos-ranlib CFLAGS="-fomit-frame-pointer -m68020 -O3"


Edit file mpfr-impl.h at line 1204
\#  define MPFR_LCONV_DPTS 0 // alkis was 1

make
sudo make install`
