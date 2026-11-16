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
#include "bool_cvp.h"
#include "fc4sc.hpp"


class scope_bool_cvp : public fc4sc::scope {

public:
  SCOPE_DECL(scope_bool_cvp)

  scope_bool_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(bool_cvp)
  { }

  class bool_cvp_coverage : public covergroup {
  public :
    CG_SCOPED_CONS(bool_cvp_coverage, scope_bool_cvp) { }

    bool mode   {false};
    bool mode_n {false};

    COVERPOINT( bool, test_cvp_1,  mode, true) {
        bin<bool>("false", false),
        bin<bool>("true",  true),
    };
    COVERPOINT( bool, test_cvp_2,  mode_n, true) {
        bin<bool>("false", false),
        bin<bool>("true",  true),
    };

    bool c1 {false};
    bool c2 {false};
    bool v1 {false};
    bool v2 {false};

    COVERPOINT( bool, cvp1, v1, c1) {
        bin<bool>("false", false),
        bin<bool>("true",  true),
    };
    COVERPOINT( bool, cvp2, v2, c2) {
        bin<bool>("false", 0),
        bin<bool>("true",  1),
    };

    cross<bool, bool> cvp1_cvp2_cr      = cross<bool, bool>(this, "cvp1_cvp2_cr",      &cvp1,     &cvp2);
    cross<bool, bool, bool, bool> test_4_coverpoints_cr = cross<bool, bool, bool, bool>
    (this, "test_4_coverpoints_cr", &cvp2, &cvp1, &test_cvp_2, &test_cvp_1);



    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;

      print_coverage(test_cvp_1);
      print_coverage(test_cvp_2);
      print_coverage(cvp1);
      print_coverage(cvp2);
      print_coverage(cvp1_cvp2_cr);
      print_coverage(test_4_coverpoints_cr);

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

  }; //end of class bool_cvp_coverage

  bool_cvp_coverage  bool_cvp;
};


void generate_bool_cvp()
{

  for (uint8_t version = 0; version < 2; version++)
  {
    auto cntxt = fc4sc::global::create_new_context();
    scope_bool_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

    std::string cov_file_name;

    if (version == 0)
    {
      cov_file_name = "bool_cov_scope_a.xml";
    }
    else
    {
      cov_file_name = "bool_cov_scope_b.xml";
    }

    std::cout << "Generating " << cov_file_name << std::endl;


        mod.bool_cvp.sample(); //default values

        mod.bool_cvp.mode = true;
        mod.bool_cvp.mode_n = true;
        mod.bool_cvp.sample();
        mod.bool_cvp.c1 = true; mod.bool_cvp.c2 = true;
        //mod.bool_cvp.v1 = false; mod.bool_cvp.v2 = false;
        mod.bool_cvp.sample();
        mod.bool_cvp.v1 = true; mod.bool_cvp.v2 = true;
        mod.bool_cvp.sample();


    if (version == 0) { //file a

        mod.bool_cvp.c1 = true;
        mod.bool_cvp.c2 = true;

        mod.bool_cvp.mode   = false;
        mod.bool_cvp.mode_n = false;
        mod.bool_cvp.v1     = false;
        mod.bool_cvp.v2     = false;

        for(int i = 0; i < 10000; i++) {
            mod.bool_cvp.sample();
        }

        mod.bool_cvp.mode   = true;
        mod.bool_cvp.mode_n = false;
        mod.bool_cvp.v1     = true;
        mod.bool_cvp.v2     = false;
        mod.bool_cvp.sample();

    }
    else { //file b

        mod.bool_cvp.c1 = true;
        mod.bool_cvp.c2 = true;

        mod.bool_cvp.mode   = true;
        mod.bool_cvp.mode_n = true;
        mod.bool_cvp.v1     = true;
        mod.bool_cvp.v2     = true;

        for(int i = 0; i < 500; i++) {
            mod.bool_cvp.sample();
        }

        mod.bool_cvp.mode   = false;
        mod.bool_cvp.mode_n = true;
        mod.bool_cvp.v1     = false;
        mod.bool_cvp.v2     = true;
        mod.bool_cvp.sample();

        mod.bool_cvp.c1     = true;
        mod.bool_cvp.c2     = false;
        mod.bool_cvp.mode   = false;
        mod.bool_cvp.mode_n = false;
        mod.bool_cvp.v1     = false;
        mod.bool_cvp.v2     = false;

        for (int j = 0; j < 5000; j++) {
            mod.bool_cvp.sample();
        }

    }


    mod.bool_cvp.stop();

    mod.bool_cvp.print_coverage();

    xml_printer::coverage_save( cov_file_name, cntxt);

    fc4sc::global::delete_context(cntxt);
  }
}
