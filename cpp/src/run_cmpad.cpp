// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023 Bradley M. Bell
// ---------------------------------------------------------------------------
/*
{xrst_begin run_cmpad.cpp}

Main Program C++ Implementation
###############################

Documentation
*************
:ref:`run_cmpad-name`

{xrst_toc_hidden
   cpp/src/parse_args.cpp
   bin/cpp/xam_main.py
}
Subroutines
***********
:ref:`parse_args-name`

Example
*******
:ref:`cpp_xam_main.py-name`

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end run_cmpad.cpp}
*/
// BEGIN C++
# include <algorithm>
# include <string>
# include <iostream>
# include <cmpad/vector.hpp>
# include <cmpad/option_t.hpp>
# include <cmpad/algo/det_by_minor.hpp>
# include <cmpad/algo/an_ode.hpp>
# include <cmpad/fun_speed.hpp>
# include <cmpad/csv_speed.hpp>
# include "parse_args.hpp"

# include <cmpad/adept/gradient.hpp>
# include <cmpad/adolc/gradient.hpp>
# include <cmpad/autodiff/gradient.hpp>
# include <cmpad/cppad/gradient.hpp>
# include <cmpad/cppad_jit/gradient.hpp>
# include <cmpad/cppadcg/gradient.hpp>
# include <cmpad/sacado/gradient.hpp>

// CMPAD_PACKAGE_TEST
# define CMPAD_PACKAGE_TEST(package) \
   if( algorithm == "det_by_minor" ) \
   {  cmpad::package::gradient<cmpad::det_by_minor> grad_det; \
      double rate = cmpad::fun_speed(grad_det, option, min_time); \
      cmpad::csv_speed( \
         file_name, rate, min_time, #package , algorithm, option \
      ); \
      case_found = true; \
   } \
   else if( algorithm == "an_ode" ) \
   {  cmpad::package::gradient<cmpad::an_ode> grad_det; \
      double rate = cmpad::fun_speed(grad_det, option, min_time); \
      cmpad::csv_speed( \
         file_name, rate, min_time, #package , algorithm, option \
      ); \
      case_found = true; \
   }


// get_package_available
cmpad::vector<std::string> get_package_available(void)
{  cmpad::vector<std::string> package_available;
   package_available.push_back("double");
# if CMPAD_HAS_ADEPT
   package_available.push_back("adept");
# endif
# if CMPAD_HAS_ADOLC
   package_available.push_back("adolc");
# endif
# if CMPAD_HAS_AUTODIFF
   package_available.push_back("autodiff");
# endif
# if CMPAD_HAS_CPPAD
   package_available.push_back("cppad");
# endif
# if CMPAD_HAS_CPPAD_JIT
   package_available.push_back("cppad_jit");
# endif
# if CMPAD_HAS_CPPADCG
   package_available.push_back("cppadcg");
# endif
# if CMPAD_HAS_SACADO
   package_available.push_back("sacado");
# endif
   return package_available;
}

int main(int argc, char* argv[])
{  //
   // arguments
   arguments_t arguments = parse_args(argc, argv);
   //
   // package_vec
   cmpad::vector<std::string> package_vec = get_package_available();
   //
   // algorithm_vec
   cmpad::vector<std::string> algorithm_vec;
   algorithm_vec.push_back("det_by_minor");
   algorithm_vec.push_back("an_ode");
   //
   // itr
   cmpad::vector<std::string>::iterator itr;
   //
   // file_name
   std::string file_name = arguments.file_name;
   //
   // package
   std::string package = arguments.package;
   itr = std::find(package_vec.begin(), package_vec.end(), package);
   if( itr == package_vec.end() )
   {  std::cerr << "run_cmpad Error: ";
      std::cerr << "package = " << package << " is not available" << "\n";
      return 1;
   }
   //
   // algorithm
   std::string algorithm = arguments.algorithm;
   itr = std::find(algorithm_vec.begin(), algorithm_vec.end(), algorithm);
   if( itr == algorithm_vec.end() )
   {  std::cerr << "run_cmpad Error: ";
      std::cerr << "algorithm = " << algorithm << " is not available" << "\n";
      return 1;
   }
   //
   // n_arg
   size_t n_arg = arguments.n_arg;
   if( algorithm == "det_by_minor" )
   {  size_t ell = size_t( std::sqrt( double(n_arg) ) );
      if( ell * ell != n_arg )
         ++ell;
      if( ell * ell != n_arg )
      {  std::cerr << "run_cmpad Error: algorithm = " << algorithm
                   << ": n_arg = " << n_arg << " is not a square.\n";
         return 1;
      }
   }
   //
   // time_setup
   bool time_setup = arguments.time_setup;
   //
   // min_time
   double min_time = arguments.min_time;
   if( min_time <= 0.0 || 1.0 < min_time )
   {  std::cerr << "run_cmpad Error: min_time = ";
      std::cerr << min_time <<
         " is less than or equal zero or greater than one.\n";
      return 1;
   }
   //
   // option
   cmpad::option_t option;
   option.n_arg       = n_arg;
   option.time_setup = time_setup;
   //
   // case_found
   bool case_found = false;
   //
   // file_name, case_found
   if( package == "double" )
   {  if( algorithm == "det_by_minor" )
      {  cmpad::det_by_minor<double> det;
         double rate = cmpad::fun_speed(det, option, min_time);
         cmpad::csv_speed(
            file_name, rate, min_time, "double", algorithm, option
         );
         case_found = true;
      }
      if( algorithm == "an_ode" )
      {  cmpad::an_ode<double> det;
         double rate = cmpad::fun_speed(det, option, min_time);
         cmpad::csv_speed(
            file_name, rate, min_time, "double", algorithm, option
         );
         case_found = true;
      }
   }
   //
   // file_name, case_found
# if CMPAD_HAS_ADEPT
   else if( package == "adept" )
   {  CMPAD_PACKAGE_TEST(adept) }
# endif
# if CMPAD_HAS_ADOLC
   else if( package == "adolc" )
   {  CMPAD_PACKAGE_TEST(adolc) }
# endif
   //
   // file_name, case_found
# if CMPAD_HAS_AUTODIFF
   else if( package == "autodiff" )
   {  CMPAD_PACKAGE_TEST(autodiff) }
# endif
   //
   // file_name, case_found
# if CMPAD_HAS_CPPAD
   else if( package == "cppad" )
   {  CMPAD_PACKAGE_TEST(cppad) }
# endif
   //
   // file_name, case_found
# if CMPAD_HAS_CPPAD_JIT
   else if( package == "cppad_jit" )
   {  CMPAD_PACKAGE_TEST(cppad_jit) }
# endif
   //
   // file_name, case_found
# if CMPAD_HAS_CPPADCG
   else if( package == "cppadcg" )
   {  CMPAD_PACKAGE_TEST(cppadcg) }
# endif
   //
   // file_name, case_found
# if CMPAD_HAS_SACADO
   else if( package == "sacado" )
   {  CMPAD_PACKAGE_TEST(sacado) }
# endif
   //
   if( case_found )
      return  0;
   //
   std::cerr << "cmpad: BUG\n";
   return 1;
}
// END C++
