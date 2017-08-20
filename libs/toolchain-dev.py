#!/usr/bin/python -B

# Build development cross toolchain for m68k-elf target.
import sys
from fnmatch import fnmatch
from os import environ

import argparse
import logging
import platform
import os

URLS = \
  [
    'https://ftp.gnu.org/gnu/gmp/gmp-6.1.2.tar.bz2',
    'http://www.mpfr.org/mpfr-current/mpfr-3.1.5.tar.bz2'
  ]


from common import * # NOQA


def build():
  for var in environ.keys():
    if var not in ['PATH','_', 'LOGNAME', 'HOME', 'SHELL', 'TMPDIR', 'PWD']:
      del environ[var]

  #environ['PATH'] = '/usr/bin:/bin:/usr/local/amiga/bin'
  #environ['PATH'] = os.environ['PATH']
  environ['LANG'] = 'C'
  environ['TERM'] = 'xterm'

  if platform.system() == 'Darwin':
    cc, cxx = 'clang', 'clang++'
  else:
    cc, cxx = 'gcc', 'g++'

  environ['CC'] = find_executable(cc)
  environ['CXX'] = find_executable(cxx)

  find_executable('bison')
  find_executable('flex')
  find_executable('make')
  find_executable('git')

  environ['PATH'] = ":".join([path.join('{target}', 'bin'),
                              path.join('{host}', 'bin'),
                              environ['PATH']])

  with cwd('{archives}'):
    for url in URLS:
      if type(url) == tuple:
        url, name = url[0], url[1]
      else:
        name = path.basename(url)
      fetch(name, url)

      
  unpack('{gmp}')
  patch('{gmp}')
  configure('{gmp}',
            '--host=x86_64-pc-linux-gnu',
            '--enable-assembly=no',
            '--enable-shared=no',
            'CFLAGS=-noixemul -fomit-frame-pointer -O3',
            'CC=m68k-amigaos-gcc',
            'NM=m68k-amigaos-nm',
            'AR=m68k-amigaos-ar',
            'RANLIB=m68k-amigaos-ranlib',
            '--prefix={target}')
  make('{gmp}')
  make('{gmp}', 'install')

  make('{gmp}', 'clean')
  rmtree('{stamps}/{gmp}-configure')
  rmtree('{stamps}/{gmp}-make')
  configure('{gmp}',
            '--host=x86_64-pc-linux-gnu',
            '--enable-assembly=no',
            '--enable-shared=no',
            'CFLAGS=-noixemul -fomit-frame-pointer -O3 -m68020',
            'CC=m68k-amigaos-gcc',
            'NM=m68k-amigaos-nm',
            'AR=m68k-amigaos-ar',
            'RANLIB=m68k-amigaos-ranlib',
            '--prefix={target}')
  make('{gmp}')
  copy('{build}/{gmp}/.libs/libgmp.a','{target}/lib/libgmp020.a')

  unpack('{mpfr}')
  configure('{mpfr}',
             '--host=x86_64-pc-linux-gnu',
            '--enable-assembly=no',
            '--enable-shared=no',
            'CFLAGS=-noixemul -fomit-frame-pointer -O3 -D_MPFR_H_HAVE_FILE -DMPFR_LCONV_DPTS=0 -I{target}/include -L{target}/lib',
            'CC=m68k-amigaos-gcc',
            'NM=m68k-amigaos-nm',
            'AR=m68k-amigaos-ar',
            'RANLIB=m68k-amigaos-ranlib',
            '--prefix={target}')
  make('{mpfr}')
  make('{mpfr}', 'install')

  #have to find a workarround to link the below with libgmp020.a  
  make('{mpfr}', 'clean')
  rmtree('{stamps}/{mpfr}-configure')
  rmtree('{stamps}/{mpfr}-make')
  configure('{mpfr}',
             '--host=x86_64-pc-linux-gnu',
            '--enable-assembly=no',
            '--enable-shared=no',
            'CFLAGS=-noixemul -fomit-frame-pointer -O3 -D_MPFR_H_HAVE_FILE -DMPFR_LCONV_DPTS=0 -m68020 -I{target}/include -L{target}/lib',
            'CC=m68k-amigaos-gcc',
            'NM=m68k-amigaos-nm',
            'AR=m68k-amigaos-ar',
            'RANLIB=m68k-amigaos-ranlib',
            '--prefix={target}')
  move('{target}/lib/libgmp.a','{target}/lib/libgmp.a_orig');
  copy('{target}/lib/libgmp020.a','{target}/lib/libgmp.a');
  make('{mpfr}')
  copy('{build}/{mpfr}/src/.libs/libmpfr.a', '{target}/lib/libmpfr020.a')
  move('{target}/lib/libgmp.a_orig','{target}/lib/libgmp.a');

def clean():
  rmtree('{stamps}')
  rmtree('{host}')
  rmtree('{build}')


if __name__ == "__main__":
  logging.basicConfig(level=logging.DEBUG, format='%(levelname)s: %(message)s')

  if not sys.version_info[:2] == (2, 7):
    panic('I need Python 2.7 to run!')

  if not (platform.system() in ['Darwin', 'Linux'] or
          fnmatch(platform.system(), 'CYGWIN*')):
    panic('Build on %s not supported!', platform.system())

  if platform.machine() not in ['i686', 'x86_64']:
    panic('Build on %s architecture not supported!', platform.machine())

  parser = argparse.ArgumentParser(description='Build cross toolchain.')
  parser.add_argument('action', choices=['build', 'clean'], default='build',
                      help='perform action')
  parser.add_argument('--prefix', type=str, default=None,
                      help='installation directory')
  args = parser.parse_args()

  setvar(top=path.abspath('.'))

  setvar(gmp='gmp-6.1.2',
         mpfr='mpfr-3.1.5',
         patches=path.join('{top}', 'patches'),
         stamps=path.join('{top}', '.build-dev', 'stamps'),
         build=path.join('{top}', '.build-dev', 'build'),
         sources=path.join('{top}', '.build-dev', 'sources'),
         host=path.join('{top}', '.build-dev', 'host'),
         tmpdir=path.join('{top}', '.build-dev', 'tmp'),
         target=path.join('{top}', 'm68k-elf'),
         archives=path.join('{top}', '.build-dev', 'archives'))

  if args.prefix is not None:
    setvar(target=args.prefix)

  if not path.exists('{target}'):
    mkdir('{target}')

  eval(args.action + "()")
