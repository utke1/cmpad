// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023 Bradley M. Bell
// ---------------------------------------------------------------------------
/*
{xrst_begin gradient.cpp}

gradient: Example and Test
##########################
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end  gradient.cpp}
*/
// BEGIN C++
# include <boost/test/unit_test.hpp>
# include <cmpad/configure.hpp>
# include <cmpad/det_by_minor.hpp>
# include <cmpad/adolc/gradient.hpp>
# include <cmpad/cppad/gradient.hpp>
# include <cmpad/sacado/gradient.hpp>
# include <cmpad/autodiff/gradient.hpp>
# include <cmpad/cppad_jit/gradient.hpp>

namespace {
   template <class Algo>
   void check_grad_det(
      const std::string&     name     ,
      cmpad::gradient<Algo>& grad_det )
   {  //
      // message
      BOOST_TEST_MESSAGE( "   " + name );
      //
      // option
      cmpad::option_t option;
      option.size = 3;
      //
      // ell
      size_t ell = 3;
      //
      // grad_det
      grad_det.setup(option);
      //
      // x
      // values in the matrix in row major order
      cmpad::vector<double> x = {
         1., 2., 3.,
         4., 5., 6.,
         7., 8., 10.
      };
      //
      // g
      cmpad::vector<double> g = grad_det(x);
      //
      BOOST_CHECK( g[ 0 * ell + 0] == + (5.*10. - 6.*8.) );
      BOOST_CHECK( g[ 0 * ell + 1] == - (4.*10. - 6.*7.) );
      BOOST_CHECK( g[ 0 * ell + 2] == + (4.*8.  - 5.*7.) );
      //
      BOOST_CHECK( g[ 1 * ell + 0] == - (2.*10. - 3.*8.) );
      BOOST_CHECK( g[ 1 * ell + 1] == + (1.*10. - 3.*7.) );
      BOOST_CHECK( g[ 1 * ell + 2] == - (1.*8.  - 2.*7.) );
      //
      BOOST_CHECK( g[ 2 * ell + 0] == + (2.*6.  - 3.*5.) );
      BOOST_CHECK( g[ 2 * ell + 1] == - (1.*6.  - 3.*4.) );
      BOOST_CHECK( g[ 2 * ell + 2] == + (1.*5.  - 2.*4.) );
   }
}

BOOST_AUTO_TEST_CASE(Gradient)
{  //
   // det_by_minor
   using cmpad::det_by_minor;
   //
# if CMPAD_HAS_ADOLC
   cmpad::adolc::gradient<det_by_minor>     adolc_grad_det;
   check_grad_det("adolc gradient",         adolc_grad_det);
# endif
   //
# if CMPAD_HAS_CPPAD
   //
   // cppad
   cmpad::cppad::gradient<det_by_minor>     cppad_grad_det;
   check_grad_det("cppad gradient",         cppad_grad_det);
   //
   // cppad_jit
   cmpad::cppad_jit::gradient<det_by_minor> cppad_jit_grad_det;
   check_grad_det("cppad_jit gradient",     cppad_jit_grad_det);
# endif
   //
# if CMPAD_HAS_SACADO
   cmpad::sacado::gradient<det_by_minor>    sacado_grad_det;
   check_grad_det("sacado gradient",        sacado_grad_det);
# endif
   //
# if CMPAD_HAS_AUTODIFF
   cmpad::autodiff::gradient<det_by_minor>  autodiff_grad_det;
   check_grad_det("autodiff gradient",      autodiff_grad_det);
# endif
}
// END C++
