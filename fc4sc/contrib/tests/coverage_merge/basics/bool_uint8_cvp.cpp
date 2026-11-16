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
#include "bool_uint8_cvp.h"
#include "fc4sc.hpp"


class scope_bool_uint8_cvp : public fc4sc::scope {

public:
  SCOPE_DECL(scope_bool_uint8_cvp)

  scope_bool_uint8_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(bool_uint8_cvp)
  { }

  class bool_uint8_cvp_coverage : public covergroup {
  public :
    CG_SCOPED_CONS(bool_uint8_cvp_coverage, scope_bool_uint8_cvp) { }


  bool    mode   {false};
  uint8_t mode_n {0};

  COVERPOINT( bool, test_cvp,  mode, true) {
    bin<bool>("false", false),
    bin<bool>("true",  true),
  };

  COVERPOINT( uint8_t, intervall_cvp,  mode_n, true) {
    bin<        uint8_t>("zero",           0),
    bin<        uint8_t>("small",         interval(1, 5)),
    bin<        uint8_t>("larger",         6, interval(7, 10)),
    bin<        uint8_t>("larger2",       11, 12, interval(13, 15), 16),
    bin_array<  uint8_t>("upper_values",   3, interval(17, 55)),
    ignore_bin< uint8_t>("ii",            67), //ignore 67
    illegal_bin<uint8_t>("illegal mode", 255)
  };

  bool c1 {false};
  bool c2 {false};
  bool v1 {false}; //bool
  uint8_t v2 {0};
  COVERPOINT( bool, cvp1, v1, c1) {
    bin<bool>("zero", false),
    bin<bool>("one",  true),
  };
  COVERPOINT( uint8_t, cvp2, v2, c2) {
    bin<uint8_t>("zero", 0),
    bin<uint8_t>("one",  1),
    bin<uint8_t>("two",  2)
  };

  cross<bool, uint8_t> cvp1_cvp2_cr      = cross<bool, uint8_t>(this, "cvp1_cvp2_cr",      &cvp1,     &cvp2);
  cross<bool, uint8_t> test_intervall_cr = cross<bool, uint8_t>(this, "test_intervall_cr", &test_cvp, &intervall_cvp);


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

  }; //end of class bool_uint8_cvp_coverage

  bool_uint8_cvp_coverage  bool_uint8_cvp;
};


void generate_bool_uint8_cvp()
{

  for (uint8_t version = 0; version < 2; version++)
  {
    auto cntxt = fc4sc::global::create_new_context();
    scope_bool_uint8_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

    std::string cov_file_name;

    if (version == 0)
    {
      cov_file_name = "bool_uint8_cov_scope_a.xml";
    }
    else
    {
      cov_file_name = "bool_uint8_cov_scope_b.xml";
    }

    std::cout << "Generating " << cov_file_name << std::endl;

    //test:
    if (version == 0) //file "bool_uint8_cov_scope_a.xml";
  {
    mod.bool_uint8_cvp.mode_n = 67; //test ignore bin
    mod.bool_uint8_cvp.sample();

    mod.bool_uint8_cvp.mode = false;
    mod.bool_uint8_cvp.mode_n = 35;
    mod.bool_uint8_cvp.sample(); //intervall_cvp x test_cvp {5,0}

    mod.bool_uint8_cvp.mode = true;
    mod.bool_uint8_cvp.mode_n = 14;
    mod.bool_uint8_cvp.sample(); //intervall_cvp x test_cvp {3,1}

    mod.bool_uint8_cvp.mode = false;
    mod.bool_uint8_cvp.mode_n = 0;
    mod.bool_uint8_cvp.sample(); //intervall_cvp x test_cvp {0,0}

    mod.bool_uint8_cvp.mode = true;
    mod.bool_uint8_cvp.mode_n = 1;
    mod.bool_uint8_cvp.sample(); //intervall_cvp x test_cvp {1,1}


  }
  else // version == 1 (file b)
  {
    mod.bool_uint8_cvp.mode = false;
    mod.bool_uint8_cvp.sample();
    mod.bool_uint8_cvp.sample();
    mod.bool_uint8_cvp.sample(); //3x intervall_cvp x test_cvp {0,0}

    mod.bool_uint8_cvp.mode = true;
    mod.bool_uint8_cvp.mode_n = 3;
    mod.bool_uint8_cvp.sample(); //intervall_cvp x test_cvp {1,1}
  }


  if (version == 0) //(file a)
  {
    mod.bool_uint8_cvp.v1 = true;
    mod.bool_uint8_cvp.v2 = 2;
    mod.bool_uint8_cvp.c1 = true;
    mod.bool_uint8_cvp.c2 = true;
    mod.bool_uint8_cvp.sample(); //cvp2 x cvp1 {2,1}

  }
  else // version == 1 (file b)
  {
    mod.bool_uint8_cvp.v1 = false;
    mod.bool_uint8_cvp.v2 = 1;
    mod.bool_uint8_cvp.c1 = false; //condition 1 false..
    mod.bool_uint8_cvp.c2 = true;
    mod.bool_uint8_cvp.sample(); //without cvp2 x cvp1 {1,0}

  }

  if (version == 0) //(file a)
  {
    mod.bool_uint8_cvp.c1 = true; mod.bool_uint8_cvp.c2 = true; //conditions true

    mod.bool_uint8_cvp.v1 = false; mod.bool_uint8_cvp.v2 = 0; // cvp2 x cvp1 {0,0}
    mod.bool_uint8_cvp.sample();

    mod.bool_uint8_cvp.v1 = true; mod.bool_uint8_cvp.v2 = 1; // cvp2 x cvp1 {1,1}
    mod.bool_uint8_cvp.sample();

  }
  else // version == 1 (file b)
  {
    mod.bool_uint8_cvp.c1 = true; mod.bool_uint8_cvp.c2 = true; //conditions true

    mod.bool_uint8_cvp.v1 = false; mod.bool_uint8_cvp.v2 = 1; // cvp2 x cvp1 {1,0}
    mod.bool_uint8_cvp.sample();

    mod.bool_uint8_cvp.v2 = 2; // cvp2 x cvp1 {2,0}
    mod.bool_uint8_cvp.sample();

    mod.bool_uint8_cvp.v1 = true; mod.bool_uint8_cvp.v2 = 0; // cvp2 x cvp1 {0,1}
    mod.bool_uint8_cvp.sample();

    mod.bool_uint8_cvp.v2 = 2; // cvp2 x cvp1 {2,1}
    mod.bool_uint8_cvp.sample();
  }


    mod.bool_uint8_cvp.sample(); //sample file a & file b
    mod.bool_uint8_cvp.stop();

    mod.bool_uint8_cvp.print_coverage();

    xml_printer::coverage_save( cov_file_name, cntxt);

    fc4sc::global::delete_context(cntxt);
  }
}
