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
#include "uint32_cvp.h"
#include "fc4sc.hpp"


class scope_uint32_cvp : public fc4sc::scope {

public:
  SCOPE_DECL(scope_uint32_cvp)

  scope_uint32_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(uint32_cvp)
  { }

  class uint32_cvp_coverage : public covergroup {
  public :
    CG_SCOPED_CONS(uint32_cvp_coverage, scope_uint32_cvp) { }


    uint32_t  mode   {0};

  COVERPOINT( uint32_t, test_cvp,  mode, true) {
    bin<uint32_t>("zero", 0),
    bin<uint32_t>("one",  1),
    bin<uint32_t>("two",  2),
    bin<uint32_t>("large", 150000)
  };

  COVERPOINT( uint32_t, intervall_cvp,  mode, true) {
    bin<        uint32_t>("zero",           0),
    bin<        uint32_t>("small",         interval(1U, 100U)),
    bin<        uint32_t>("larger",        interval(101U, 1000U)),
    bin<        uint32_t>("larger2",       1001U, interval(1002U, 10000U)),
    bin_array<  uint32_t>("upper_values",  5, interval(10001U, 4294967294U)), //U...unsigned
    ignore_bin< uint32_t>("ii",            15067U),
    illegal_bin<uint32_t>("illegal mode", 4294967295U)
  };

  bool c1 {false};
  bool c2 {false};
  uint32_t v1 {0};
  uint32_t v2 {0};
  COVERPOINT( uint32_t, cvp1, v1, c1) {
    bin<uint32_t>("zero", 1000),
    bin<uint32_t>("one",  10000),
    bin<uint32_t>("two",  100000)
  };
  COVERPOINT( uint32_t, cvp2, v2, c2) {
    bin<uint32_t>("zero", 0),
    bin<uint32_t>("one",  1),
    bin<uint32_t>("two",  2)
  };

  cross<uint32_t, uint32_t> cvp1_cvp2_cr      = cross<uint32_t, uint32_t>(this, "cvp1_cvp2_cr",      &cvp1,     &cvp2);
  cross<uint32_t, uint32_t> test_intervall_cr = cross<uint32_t, uint32_t>(this, "test_intervall_cr", &test_cvp, &intervall_cvp);
  cross<uint32_t, uint32_t, uint32_t> test_3_crosses_cr = cross<uint32_t, uint32_t, uint32_t>(this, "test_3_crosses_cr", &test_cvp, &intervall_cvp, &cvp2);


    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;

      print_coverage(test_cvp);
      print_coverage(intervall_cvp);
      print_coverage(cvp1);
      print_coverage(cvp2);
      print_coverage(cvp1_cvp2_cr);
      print_coverage(test_intervall_cr);
      print_coverage(test_3_crosses_cr);

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

  }; //end of class uint32_cvp_coverage

  uint32_cvp_coverage  uint32_cvp;
};


void generate_uint32_cvp()
{

  for (uint8_t version = 0; version < 2; version++)
  {
    auto cntxt = fc4sc::global::create_new_context();
    scope_uint32_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

    std::string cov_file_name;

    if (version == 0)
    {
      cov_file_name = "uint32_cov_scope_a.xml";
    }
    else
    {
      cov_file_name = "uint32_cov_scope_b.xml";
    }

    std::cout << "Generating " << cov_file_name << std::endl;


    mod.uint32_cvp.mode = 4294967294; mod.uint32_cvp.sample();
    //mod.uint32_cvp.mode = 4294967295; mod.uint32_cvp.sample(); //illegal mode
    mod.uint32_cvp.mode = 15067; mod.uint32_cvp.sample(); //ignore bin


    mod.uint32_cvp.mode = 150000; mod.uint32_cvp.sample();
    mod.uint32_cvp.c1 = false; mod.uint32_cvp.c2 = true;
    mod.uint32_cvp.v1 = 10000; mod.uint32_cvp.v2 = 2;
    mod.uint32_cvp.sample();

    mod.uint32_cvp.c1 = true; mod.uint32_cvp.c2 = true; //merge error if no "cvp1_cvp2_cr" hit !!

    if (version == 0) // file a
    {
      // test_cvp & intervall_cvp
      mod.uint32_cvp.mode = 0; mod.uint32_cvp.sample();
      mod.uint32_cvp.mode = 1; mod.uint32_cvp.sample();
      mod.uint32_cvp.mode = 50; mod.uint32_cvp.sample();

      // cvp1_cvp2_cr
      mod.uint32_cvp.v1 = 1000;
      mod.uint32_cvp.v2 = 0;
      mod.uint32_cvp.sample();
    }
    else { //file b
      // test_cvp & intervall_cvp
      mod.uint32_cvp.mode = 2; mod.uint32_cvp.sample();
      mod.uint32_cvp.mode = 500; mod.uint32_cvp.sample();
      mod.uint32_cvp.mode = 20000; mod.uint32_cvp.sample();

      // cvp1_cvp2_cr
      mod.uint32_cvp.v1 = 100000;
      mod.uint32_cvp.v2 = 2;
      mod.uint32_cvp.sample();
    }

    mod.uint32_cvp.stop();

    mod.uint32_cvp.print_coverage();

    xml_printer::coverage_save( cov_file_name, cntxt);

    fc4sc::global::delete_context(cntxt);
  }
}
