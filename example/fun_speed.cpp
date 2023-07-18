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
# include <cmpad/det_by_minor.hpp>
# include <cmpad/adolc/gradient.hpp>
# include <cmpad/cppad/gradient.hpp>
# include <cmpad/fun_speed.hpp>

namespace {
   template <class Algo>
   void check_speed_det(cmpad::gradient<Algo>& grad_det)
   {  //
      //
      // time_min
      // minimum time for test in seconds
      double time_min = 0.2;
      //
      // previous_rate, ell
      double previous_rate = 0.0;
      for(size_t ell = 5; ell < 10; ++ell)
      {  //
         // rate  
         double rate  = cmpad::fun_speed(grad_det, ell, time_min);
         //
         // ratio
         // number of floating operations goes up by a factor of ell
         double ratio = previous_rate / (ell * rate); 
         //
         if( previous_rate != 0.0 )
            BOOST_CHECK( 0.5 < ratio && ratio < 2.0 );
      }
   }
}

BOOST_AUTO_TEST_CASE(Fun_speed)
{  //
   // adolc
   BOOST_TEST_MESSAGE("   adolc fun_speed");
   typedef cmpad::det_by_minor<adouble>       adolc_Algo;
   cmpad::adolc::gradient<adolc_Algo>         adolc_grad_det;
   check_speed_det(adolc_grad_det);
   //
   // cppad
   BOOST_TEST_MESSAGE("   cppad fun_speed");
   typedef cmpad::det_by_minor< CppAD::AD<double> > cppad_Algo;
   cmpad::cppad::gradient<cppad_Algo>         cppad_grad_det;
   check_speed_det(cppad_grad_det);
}
// END C++
