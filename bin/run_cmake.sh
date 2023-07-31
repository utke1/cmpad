#! /usr/bin/env bash
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2023 Bradley M. Bell
# ---------------------------------------------------------------------------
set -u -e
if [ "$0" != 'bin/run_cmake.sh' ]
then
   echo 'bin/run_cmake.sh: must be executed from its parent directory'
   exit 1
fi
#
# compiler, build_type, vector
compiler=''
build_type='-D CMAKE_BUILD_TYPE=release'
code_generator=""
vector='-D cmpad_vector=cppad'
verbose='-D CMAKE_VERBOSE_MAKEFILE=NO'
for i in $(seq 1 $#)
do
   i_th_command_line_argument="${!i}"
   case "$i_th_command_line_argument" in

      --clang)
      compiler='-D CMAKE_CXX_COMPILER=clang++'
      ;;

      --debug)
      build_type='-D CMAKE_BUILD_TYPE=debug'
      ;;

      --ninja)
      code_generator='-G Ninja'
      ;;

      --std_vector)
      vector='-D cmpad_vector=std'
      ;;

      --verbose)
      verbose='-D CMAKE_VERBOSE_MAKEFILE=YES'
      ;;

      *)
      echo 'usage: bin/run_cmake.sh \\'
      echo '   [--clang] \\'
      echo '   [--debug] \\'
      echo '   [--ninja ] \\ '
      echo '   [--std_vector] \\'
      echo '   [--verbose]'
      exit 0
      ;;

   esac
done
# -----------------------------------------------------------------------------
# build
if [ ! -e build ]
then
   mkdir build
fi
cd build
if [ -e CMakeCache.txt ]
then
   rm CMakeCache.txt
fi
#
# PKG_CONFIG_PATH
# if this is empty, cmake uses local install of packages
PKG_CONFIG_PATH=''
#
# for_sacado
for_sacado='-DCMAKE_CXX_EXTENSIONS=Off'
#
# make
echo cmake -B . -S .. \
   $code_generator $for_sacado $compiler $build_type $vector $verbose
cmake -B . -S .. \
   $code_generator $for_sacado $compiler $build_type $vector $verbose
# -----------------------------------------------------------------------------
echo 'bin/run_cmake.sh: OK'
exit 0
