// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023 Bradley M. Bell
// ---------------------------------------------------------------------------
# ifndef TEST_CHECK_SPEED_DET_HPP
# define TEST_CHECK_SPEED_DET_HPP
# include <cmpad/fun_speed.hpp>

template <class Algo>
void check_speed_det(cmpad::gradient<Algo>& grad_det )
{  //
   // option
   cmpad::option_t option;
   //
   // time_min
   // minimum time for test in seconds
   double time_min = 0.2;
   //
   // previous_rate, size
   double previous_rate = 0.0;
   for(size_t ell = 5; ell < 8; ++ell)
   {  //
      // option
      option.size       = ell;
      option.time_setup = false;
      //
      // grad_det
      grad_det.setup(option);
      //
      // rate
      double rate  = cmpad::fun_speed(grad_det, option, time_min);
      //
      // ratio
      // number of floating operations goes up by a factor of ell
      double ratio = previous_rate / (ell * rate);
      //
      if( ! (previous_rate == 0.0 || (0.1 < ratio && ratio < 3.0) ) )
      {  std::cout << "ell = " << ell << ": ratio = ";
         std::cout << std::to_string(ratio) << "\n";
      }
      if( previous_rate != 0.0 )
         BOOST_CHECK( 0.1 < ratio && ratio < 3.0 );
      previous_rate = rate;
   }
}

# endif
