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
   Authors  : Georg Christian Moosdorf, HTW Dresden, 2026-11-16
              Jens Schoenherr,          HTW Dresden, 2026-11-16
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>
#include "cross_without_bin_cvp.h"
#include "fc4sc.hpp"


class scope_cross_without_bin_cvp : public fc4sc::scope {

public:
  SCOPE_DECL(scope_cross_without_bin_cvp)

  scope_cross_without_bin_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(cross_without_bin_cvp)
  { }

  class cross_without_bin_coverage : public covergroup {
  public :
    CG_SCOPED_CONS(cross_without_bin_coverage, scope_cross_without_bin_cvp) { }


    bool c1 {false};
    bool c2 {false};
    uint8_t v1 {0};
    uint8_t v2 {0};

    COVERPOINT( uint8_t, cvp_1,  v1, c1) {
      ignore_bin<uint8_t>("two",  2)
    };

    COVERPOINT( uint8_t, cvp_2,  v2, c2) {
      bin<uint8_t>("zero", 0),
      bin<uint8_t>("one",  1),
      bin<uint8_t>("two",  2)
    };

    cross<uint8_t, uint8_t> cvp1_cvp2_cr      = cross<uint8_t, uint8_t>(this, "cvp1_cvp2_cr",      &cvp_1,     &cvp_2);


    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;

      print_coverage(cvp_1);
      print_coverage(cvp_2);
      print_coverage(cvp1_cvp2_cr);


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

  }; //end of class cross_without_bin_cvp_coverage

  cross_without_bin_coverage  cross_without_bin_cvp;
};


void generate_cross_without_bin_cvp()
{

  for (uint8_t version = 0; version < 2; version++)
  {
    auto cntxt = fc4sc::global::create_new_context();
    scope_cross_without_bin_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

    std::string cov_file_name;

    if (version == 0)
    {
      cov_file_name = "cross_without_bin_cov_scope_a.xml";
    }
    else
    {
      cov_file_name = "cross_without_bin_cov_scope_b.xml";
    }

    std::cout << "Generating " << cov_file_name << std::endl;


    if (version > 0) //file b
    {
      mod.cross_without_bin_cvp.v1 = 1;
      mod.cross_without_bin_cvp.v2 = 2;
      mod.cross_without_bin_cvp.c1 = false;
      mod.cross_without_bin_cvp.c2 = true;
      mod.cross_without_bin_cvp.sample();
    }

    mod.cross_without_bin_cvp.v1 = 1;
    mod.cross_without_bin_cvp.v2 = 0;
    mod.cross_without_bin_cvp.c1 = false;
    mod.cross_without_bin_cvp.c2 = true;
    mod.cross_without_bin_cvp.sample();

    mod.cross_without_bin_cvp.v1 = 2;
    mod.cross_without_bin_cvp.v2 = 1;
    mod.cross_without_bin_cvp.c1 = true;
    mod.cross_without_bin_cvp.c2 = false;
    mod.cross_without_bin_cvp.sample();


    mod.cross_without_bin_cvp.sample();
    mod.cross_without_bin_cvp.stop();

    mod.cross_without_bin_cvp.print_coverage();

    xml_printer::coverage_save( cov_file_name, cntxt);

    fc4sc::global::delete_context(cntxt);
  }
}
