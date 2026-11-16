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
#include "int_cvp.h"
#include "fc4sc.hpp"


class scope_int_cvp : public fc4sc::scope {

public:
  SCOPE_DECL(scope_int_cvp)

  scope_int_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(int_cvp)
  { }

  class int_cvp_coverage : public covergroup {
  public :
    CG_SCOPED_CONS(int_cvp_coverage, scope_int_cvp) { }

    int mode = 0;
    int v1 = 0;
    int v2 = 0;
    bool c1 = false;
    bool c2 = false;

    COVERPOINT( int, test_cvp, mode, true) {
        bin<int>(" pos10  ", +10),
        bin<int>(" neg10 ", -10)
    };

    COVERPOINT( int, intervall_cvp,  mode, true) {
        bin_array<  int>("negative large",   3, interval(-1000, -101)),
        bin<        int>("negative small",      interval(-100, -2), -1),
        bin<        int>("zero",                0),
        bin<        int>("positive small",      1, interval(2, 100)),
        bin_array<  int>("positive large",   3, interval(101, 1000)),
        ignore_bin <int>("ii",                  77),
        illegal_bin<int>("illegal mode", 101)
    };

    COVERPOINT ( int , cp1 , v1 , c1 ) {
        bin < int >( " pos1 " , +1) ,
        bin < int >( " neg1 " , -1)
    };
    COVERPOINT ( int , cp2 , v2 , c2 ) {
        bin < int >( " pos1 " , +1) ,
        bin < int >( " neg1 " , -1)
    };

    cross < int , int, int, int > testcvp_intervallcvp_cp1_cp2_cr =
    cross < int , int, int, int >( this , " testcvp_intervallcvp_cp1_cp2_cr " , &test_cvp, &intervall_cvp, &cp1 , &cp2);


    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;

      print_coverage(test_cvp);
      print_coverage(intervall_cvp);
      print_coverage(cp1);
      print_coverage(cp2);
      //print_coverage(cvp1_cvp2_cr);
      print_coverage(testcvp_intervallcvp_cp1_cp2_cr);

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

  }; //end of class int_cvp_coverage

  int_cvp_coverage  int_cvp;
};


void generate_int_cvp()
{

  for (uint8_t version = 0; version < 2; version++)
  {
    auto cntxt = fc4sc::global::create_new_context();
    scope_int_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

    std::string cov_file_name;

    if (version == 0)
    {
      cov_file_name = "int_cov_scope_a.xml";
    }
    else
    {
      cov_file_name = "int_cov_scope_b.xml";
    }

    std::cout << "Generating " << cov_file_name << std::endl;


    //mod.int_cvp.mode = 101; mod.int_cvp.sample(); //illegal bin
    mod.int_cvp.mode = 77; mod.int_cvp.sample(); //ignore bin
    mod.int_cvp.mode = -5; mod.int_cvp.sample();
    mod.int_cvp.mode = -999;
    mod.int_cvp.sample();
    mod.int_cvp.c1 = true; mod.int_cvp.v1 = -1; mod.int_cvp.v2 = -1;
    mod.int_cvp.sample();

    if (version == 0) { // file a
        mod.int_cvp.mode = 1;
        mod.int_cvp.v1 = -1;
        mod.int_cvp.sample();

        mod.int_cvp.c2 = true;
        mod.int_cvp.v2 = 1;
        mod.int_cvp.mode = -10;
        mod.int_cvp.sample();

    }
    else { // file b
        mod.int_cvp.mode = 999;
        mod.int_cvp.v1 = -1;
        mod.int_cvp.sample();

        mod.int_cvp.c2 = true;
        mod.int_cvp.v2 = -1;
        mod.int_cvp.mode = +10;
        mod.int_cvp.sample();
    }


    mod.int_cvp.stop();

    mod.int_cvp.print_coverage();

    xml_printer::coverage_save( cov_file_name, cntxt);

    fc4sc::global::delete_context(cntxt);
  }
}
