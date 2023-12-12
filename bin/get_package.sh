#! /usr/bin/env bash
# ---------------------------------------------------------------------------
# SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
# SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
# SPDX-FileContributor: 2023 Bradley M. Bell
# ---------------------------------------------------------------------------
set -e -u
# -----------------------------------------------------------------------------
# \{xrst_begin get_package.sh}
# {xrst_spell
#     eigen
#     pwd
#     rm
#     srcdir
# }
# {xrst_comment_ch #}
#
# Getting Optional C++ Packages
# #############################
# This shell script can be used to install optional AD packages
# that cmpad can test. In addition, it can be used to install packages
# required by these optional packages.
#
# Syntax
# ******
# ``cpp/bin/get_package.sh`` *build_type* *package_1* [ *package_2* [ ... ] ]
# ``cpp/bin/get_package.sh`` *build_type* ``all``
#
# build_type
# ==========
# This is either ``debug`` or ``release`` and determines if C++ and Clibraries,
# built while installing the packages, are debug or release versions.
#
# all
# ===
# If *package_1* is ``all`` , all of the possible packages,
# except fastad and clad (thoes under construction) are installed.
# In this case, *package_2* .. must not be present on the command line.
#
# package_j
# =========
# The packages *package_1* , *package_2*, ...
# are the list of packages that will be installed.
# This list must have at least one package ; i.e.,
# *package_2* , *package_3* , ... are optional.
#
# AD Packages
# ***********
# This is the set of AD packages (so far) that can be installed:
# If one of these packages is not install, it will not be included in
# the cmpad testing.
#
# {xrst_comment BEGIN_SORT_THIS_LINE_PLUS_1}
# .. _adept:     https://github.com/rjhogan/Adept-2
# .. _adolc:     https://github.com/coin-or/ADOL-C
# .. _autodiff:  https://github.com/autodiff/autodiff
# .. _autograd: https://github.com/HIPS/autograd.git
# .. _clad:      https://github.com/vgvassilev/clad
# .. _cppad:     https://github.com/coin-or/CppAD
# .. _cppad_jit: https://github.com/coin-or/CppAD
# .. _cppad_py: https://github.com/bradbell/cppad_py
# .. _cppadcg:   https://github.com/joaoleal/CppADCodeGen
# .. _fastad:    https://github.com/JamesYang007/FastAD
# .. _pytorch:  https://github.com/pytorch/pytorch
# .. _sacado:    https://trilinos.github.io/sacado.html
# {xrst_comment END_SORT_THIS_LINE_MINUS_1}
#
# {xrst_comment BEGIN_SORT_THIS_LINE_PLUS_6}
# .. csv-table::
#  :widths: auto
#  :header-rows: 1
#
#  Web Site,      Implemented,                                    Lanuage
#  `adept`_,      :ref:`gradient <adept_gradient.hpp-name>`,      cpp
#  `adolc`_,      :ref:`gradient <adolc_gradient.hpp-name>`,      cpp
#  `autodiff`_,   :ref:`gradient <autodiff_gradient.hpp-name>`,   cpp
#  `autograd`_,   :ref:`autograd_gradient-name`,                  python
#  `clad`_,       Under Construction,                             cpp
#  `cppad_jit`_,  :ref:`gradient <cppad_jit_gradient.hpp-name>`,  cpp
#  `cppad_py`_,   :ref:`cppad_py_gradient-name`,                  python
#  `cppad`_,      :ref:`gradient <cppad_gradient.hpp-name>`,      cpp
#  `cppadcg`_,    :ref:`gradient <cppadcg_gradient.hpp-name>`,    cpp
#  `fastad`_,     Under Construction,                             cpp
#  `pytorch`_,    :ref:`pytorch_gradient-name`,                   python
#  `sacado`_,     :ref:`gradient <sacado_gradient.hpp-name>`,     cpp
# {xrst_comment END_SORT_THIS_LINE_MINUS_1}
#
# #. Implemented is the list of cmpad derivatives implemented so far
# #. Installing autodiff or fastad also installs eigen.
# #. Installing cppad or cppad_jit has the same effect.
# #. Installing cppadcg also installs cppad.
#
# {xrst_suspend}
pkg_available='   adept, adolc, autodiff, autograd, clad, cppad_jit, cppad_py,
   cppadcg, fastad, pytorch, sacado'
# {xrst_resume}
#
# prefix
# ******
# The is the prefix for the packages installed by ``bin/get_package.sh`` is
# 
# | |tab| *top_srcdir* / *language* ``/build/prefix``
#
# external
# ********
# The source code, and corresponding builds, for all packages
# is in the *top_srcdir*\ ``/external`` directory.
# Thus you can remove the *prefix* directory and reinstall a new list
# of packages quickly.
# If you have trouble with a particular *package* ,
# and ``external/``\ *package*\ .\ *build_type* exists,
# you may want to try the following:
#
# | |tab| ``rm external/``\ *package*\ .\ *build_type*
# | |tab| ``bin/cpp/get_package`` *build_type* *package*
#
# pytorch
# =======
# The pytorch package is special because it is installed a remote copy
# of its source; i.e., it does not have a local copy of its soruce below
# the external directory.
#
# {xrst_end get_package.sh}
# -----------------------------------------------------------------------------
# echo_eval
# bash function that echos and executes a command
echo_eval() {
	echo $*
   eval $*
}
# -----------------------------------------------------------------------------
program='bin/get_package.sh'
#
if [ ! -d '.git' ]
then
   echo "$program: must be executed from cmpad top soruce directory"
   exit 1
fi
if [ "$#" -lt 2 ]
then
   echo "$program: found $# arugments and expected 2 or more"
   echo
   echo "usage: $program build_type all"
   echo "usage: $program build_type package_1 [package_2 [...] ]"
   echo 'where build_type is debug or release and package_j is one of'
   echo "$pkg_available"
   exit 1
fi
#
# build_type
build_type="$1"
shift
if [ "$build_type" != 'debug' ] && [ "$build_type" != 'release' ]
then
   echo
   echo "usage: $program build_type all"
   echo "usage: $program build_type package_1 [package_2 [...] ]"
   echo "build_type=$build_type is not debug or release"
   exit 1
fi
#
# package
package="$1"
#
# all
if [ "$package" == 'all' ]
then
   if [ "$#" -gt 1 ]
   then
      echo "usage: $program build_type all"
      echo 'package_2, ..., must not be present when package_1 is all'
      exit 1
   fi
   # exclude clad and fastad
   pkg_list=$(echo $pkg_available | sed \
      -e 's/, *clad *,/, /'  \
      -e 's/, *fastad *,/, /' \
      -e 's/,/ /g'  \
      -e 's/\n/ /g'  \
      -e 's/  */ /g' \
   )
   echo_eval $program $build_type $pkg_list
   echo "$program $build_type all: OK"
   exit 0
fi
#
# package
while [ "$#" -ge 1 ]
do
   package="$1"
   shift

   case $package in

   adept|adolc|autodiff|clad|cppad_jit|cppad|cppadcg|fastad|sacado)
   echo_eval cpp/bin/get_package.sh $build_type $package
   ;;

   autograd|cppad_py|pytorch)
   echo_eval python/bin/get_package.py $build_type $package
   ;;

   *)
   echo "bin/get_package.sh: package = $package is not available"
   exit 1

   esac
done
echo "$program $build_type $package: OK"
exit 0
