// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023 Bradley M. Bell
// ---------------------------------------------------------------------------
# ifndef CMPAD_SACADO_GRADIENT_HPP
# define CMPAD_SACADO_GRADIENT_HPP
/*
{xrst_begin sacado_gradient.hpp}

{xrst_template ,
   cpp/include/cmpad/gradient.xrst
   @Package@ , Sacado
   @#######@ , ######
   @package@ , sacado
}


{xrst_end sacado_gradient.hpp}
*/
// BEGIN C++
# if CMPAD_HAS_SACADO

# include <Sacado.hpp>
# include <cmpad/gradient.hpp>

namespace cmpad { namespace sacado { // BEGIN cmpad::sacado namespace

// gradient
template < template<class Scalar> class TemplateAlgo> class gradient
: public ::cmpad::gradient< TemplateAlgo< Sacado::Rad::ADvar<double> > > {
private:
   //
   // option_
   option_t                                            option_;
   //
   // algo_
   TemplateAlgo< Sacado::Rad::ADvar<double> >          algo_;
   //
   // ax_
   cmpad::vector< Sacado::Rad::ADvar<double> >         ax_;
   //
   // ay_
   cmpad::vector< Sacado::Rad::ADvar<double> >         ay_;
   //
   // g_
   cmpad::vector<double>                               g_;
//
public:
   // value_type
   typedef double value_type;
   // option
   const option_t& option(void) const override
   {  return option_; }
   // setup
   void setup(const option_t& option) override
   {  //
      // option_
      option_ = option;
      //
      // algo_
      algo_.setup(option);
      //
      // n, m
      size_t n = algo_.domain();
      size_t m = algo_.range();
      //
      // ax_
      ax_.resize(n);
      //
      // ay_
      ay_.resize(m);
      //
      // g_
      g_.resize(n);
   }
   // domain
   size_t domain(void) const override
   {  return algo_.domain(); };
   //
   // operator
   const cmpad::vector<double>& operator()(
      const cmpad::vector<double>& x
   ) override
   {  // ax_
      for(size_t j = 0; j < domain(); ++j)
         ax_[j] = x[j];
      //
      // ay_
      ay_ = algo_(ax_);
      //
      // az
      size_t m = algo_.range();
      Sacado::Rad::ADvar<double> az = ay_[m-1] + 0.0;
      //
      // reverse mode computation of gradient for last computed value
      Sacado::Rad::ADvar<double>::Gradcomp();
      //
      // g_
      for(size_t j = 0; j < domain(); ++j)
         g_[j] = ax_[j].adj();
      //
      return g_;
   }
};

} } // END cmpad::sacado namespace

# endif // CMPAD_HAS_SACADO
// END C++
# endif
