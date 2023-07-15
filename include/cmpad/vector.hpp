// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023 Bradley M. Bell
// ---------------------------------------------------------------------------
# ifndef CMPAD_VECTOR_HPP
# define CMPAD_VECTOR_HPP
/*
{xrst_begin cmpad_vector}
{xrst_spell
   cmake
   cppad
   std
}

The cmpad::vector Template Class
################################

cmpad::vector
*************
The cmpad cmake command line can define cmpad_vector in the following way:

|   ``cmake -D cmpad_vector=`` *type*

where *type* is one of the following values: ``boost`` , ``cppad`` ,
or ``std``.
This will determine if ``cmpad::vector`` is the boost, cppad, or std
vector template class.
The default value, if *cmpad_vector* is not defined on the cmake command line,
is ``std`` .

{xrst_end cmpad_vector}
*/


// boost vector
# if 0
# include<boost/numeric/ublas/vector.hpp>
namespace cmpad { using boost::numeric::ublas::vector; }
# endif

// CppAD vector
# if 1
# include<cppad/utility/vector.hpp>
namespace cmpad { using CppAD::vector; }
# endif

// std vector
# if 0
# include<vector>
namespace cmpad { using std::vector; }
# endif


# endif
