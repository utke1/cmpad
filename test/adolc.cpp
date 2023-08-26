// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <boost/test/unit_test.hpp>
# include <cmpad/algo/det_by_minor.hpp>
# include <cmpad/adolc/gradient.hpp>
# include <cmpad/csv_speed.hpp>
# include "check_speed_det.hpp"
# include "csv_fixture.hpp"

BOOST_AUTO_TEST_SUITE(fun_speed)
BOOST_AUTO_TEST_CASE(adolc)
{  cmpad::adolc::gradient<cmpad::det_by_minor> grad_det;
   check_speed_det(grad_det);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_FIXTURE_TEST_SUITE(csv_speed, csv_fixture)
BOOST_AUTO_TEST_CASE(adolc)
{  //
   // file_name
   std::string file_name = csv_file_path_;
   //
   // time_setup
   for(bool time_setup : {true, false} )
   {
      // option
      cmpad::option_t option;
      option.size       = size_;
      option.time_setup = time_setup;
      //
      // det
      BOOST_REQUIRE( algorithm_ == "det_by_minor" );
      cmpad::adolc::gradient<cmpad::det_by_minor> grad_det;
      //
      // rate
      double rate = cmpad::fun_speed(grad_det, option, time_min_);
      //
      // csv_speed
      cmpad::csv_speed(file_name, rate, "adolc", algorithm_, option);
   }
   check("adolc");
}
BOOST_AUTO_TEST_SUITE_END()
