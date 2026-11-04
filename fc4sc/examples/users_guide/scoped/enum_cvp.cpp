/******************************************************************************

   Copyright 2026 HTW Dresden

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

******************************************************************************/
/******************************************************************************
   Authors  : Jens Schoenherr,          HTW Dresden, 2026-11-03
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>
#include "fc4sc.hpp"
#include "enum_cvp.h"
#include "context_info.h"

typedef enum { aaa, bbb, ccc} test_t;

enum class test_ec_impl            { ddd, eee, fff};
enum class test_ec_int   : int     { ggg, hhh, iii};
enum class test_ec_uint8 : uint8_t { jjj, kkk, lll};

class scope_enum : public fc4sc::scope
{

public:
  SCOPE_DECL(scope_enum)

  scope_enum(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(enum_cvg)
  { }

  class enum_cvp_coverage : public covergroup
  {
  public :

    CG_SCOPED_CONS(enum_cvp_coverage, scope_enum) { }

    test_t enum_var;
/*  enums are not supported for coverpoints
    COVERPOINT(test_t, enum_cvp, enum_var, true)
    {
      bin<test_t>("aaa", aaa),
      bin<test_t>("bbb", bbb),
      bin<test_t>("ccc", ccc)
    };
*/
    // workaround for enums
    COVERPOINT(int, enum_wa_cvp, enum_var, true)
    {
      bin<int>("aaa", aaa),
      bin<int>("bbb", bbb),
      bin<int>("ccc", ccc)
    };
    test_ec_impl  ec_impl_var  {test_ec_impl::ddd};
    test_ec_int   ec_int_var   {test_ec_int::ggg};
    test_ec_uint8 ec_uint8_var {test_ec_uint8::jjj};

    typedef std::underlying_type<test_ec_impl >::type test_ec_impl_t;
    typedef std::underlying_type<test_ec_int  >::type test_ec_int_t;
    typedef std::underlying_type<test_ec_uint8>::type test_ec_uint8_t;

/*  enum classes are not supported for coverpoints
    COVERPOINT(test_ec_impl, enum_ec_impl_cvp, ec_impl_var, true)
    {
      bin<test_ec_impl>("ddd", test_ec_impl::ddd),
      bin<test_ec_impl>("eee", test_ec_impl::eee),
      bin<test_ec_impl>("fff", test_ec_impl::fff)
    };
    COVERPOINT(test_ec_int, enum_ec_int_cvp, ec_int_var, true)
    {
      bin<test_ec_int>("ggg", test_ec_int::ggg),
      bin<test_ec_int>("hhh", test_ec_int::hhh),
      bin<test_ec_int>("iii", test_ec_int::iii)
    };
    COVERPOINT(test_ec_uint8, enum_ec_uint8_cvp, ec_uint8_var, true)
    {
      bin<test_ec_uint8>("jjj", test_ec_uint8::jjj),
      bin<test_ec_uint8>("kkk", test_ec_uint8::kkk),
      bin<test_ec_uint8>("lll", test_ec_uint8::lll)
    };
*/
    // workaround: use underlying type
    COVERPOINT(test_ec_impl_t, enum_ec_impl_wa_cvp, static_cast<test_ec_impl_t>(ec_impl_var), true)
    {
      bin<test_ec_impl_t>("ddd", static_cast<test_ec_impl_t>(test_ec_impl::ddd)),
      bin<test_ec_impl_t>("eee", static_cast<test_ec_impl_t>(test_ec_impl::eee)),
      bin<test_ec_impl_t>("fff", static_cast<test_ec_impl_t>(test_ec_impl::fff))
    };
    COVERPOINT(test_ec_int_t, enum_ec_int_wa_cvp, static_cast<test_ec_int_t>(ec_int_var), true)
    {
      bin<test_ec_int_t>("ggg", static_cast<test_ec_int_t>(test_ec_int::ggg)),
      bin<test_ec_int_t>("hhh", static_cast<test_ec_int_t>(test_ec_int::hhh)),
      bin<test_ec_int_t>("iii", static_cast<test_ec_int_t>(test_ec_int::iii))
    };
    COVERPOINT(test_ec_uint8_t, enum_ec_uint8_wa_cvp, static_cast<test_ec_uint8_t>(ec_uint8_var), true)
    {
      bin<test_ec_uint8_t>("jjj", static_cast<test_ec_uint8_t>(test_ec_uint8::jjj)),
      bin<test_ec_uint8_t>("kkk", static_cast<test_ec_uint8_t>(test_ec_uint8::kkk)),
      bin<test_ec_uint8_t>("lll", static_cast<test_ec_uint8_t>(test_ec_uint8::lll))
    };

    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;

      print_coverage(enum_wa_cvp);
      print_coverage(enum_ec_impl_wa_cvp);
      print_coverage(enum_ec_int_wa_cvp);
      print_coverage(enum_ec_uint8_wa_cvp);

      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;
    }

    template <typename... T1>
    void print_coverage(cross<T1...> cr) const
    {
      std::cout << std::setw(7) << cr.get_inst_coverage() << " % " << cr.name() << std::endl;
      std::cout << "( ";
      for (auto i : cr.cvps_vec)
      {
        std::cout << i->name() << "  ";
      }
      std::cout << " )" << std::endl;
      for (auto i : cr.get_cross_bins())
      {
        auto vec = i.first;
        auto num = i.second;
        std::cout << "( ";
        for (auto ve : vec)
        {
          std::cout << std::setw(3) << ve << " ";
        }
        std::cout << " ) : " << std::setw(4) << num << std::endl;
      }
    }

    template <typename T>
    void print_coverage(coverpoint<T> cvp) const
    {
      std::cout << std::setprecision(3) << std::fixed;
      std::cout << std::setw(7) << cvp.get_inst_coverage() << " % " << cvp.name() << std::endl;
      for (uint64_t i = 0; i < cvp.size(); i++)
      {
        std::cout << "( " << std::setw(4) << i << " ) : " << std::setw(4) << cvp.get_bin_hit_count(i)  << std::endl;
      }
    }

  }; // end of class enum_cvp_coverage

  enum_cvp_coverage  enum_cvg;
};

void generate_enum_cvp()
{
  std::string cov_file_name("enum_cvp_coverage");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  fc4sc::global *cntxt = fc4sc::global::create_new_context();
  scope_enum mod("mod", __FILE__, __LINE__, cntxt);

  mod.enum_cvg.enum_var     = aaa;
  mod.enum_cvg.ec_impl_var  = test_ec_impl::ddd;
  mod.enum_cvg.ec_int_var   = test_ec_int::ggg;
  mod.enum_cvg.ec_uint8_var = test_ec_uint8::jjj;
  mod.enum_cvg.sample();

  mod.enum_cvg.enum_var     = bbb;
  mod.enum_cvg.ec_impl_var  = test_ec_impl::eee;
  mod.enum_cvg.ec_int_var   = test_ec_int::hhh;
  mod.enum_cvg.ec_uint8_var = test_ec_uint8::kkk;
  mod.enum_cvg.sample();

  mod.enum_cvg.stop();

  mod.enum_cvg.print_coverage();

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  print_context_info(cntxt);

  fc4sc::global::delete_context(cntxt);
}
