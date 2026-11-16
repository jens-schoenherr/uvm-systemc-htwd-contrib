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
#include "without_cross_test_cvp.h"
#include "fc4sc.hpp"

    const int RED    = 0;
    const int YELLOW = 1;
    const int GREEN  = 2;

class scope_traffic_light_test_cvp : public fc4sc::scope {

public:
  SCOPE_DECL(scope_traffic_light_test_cvp)

  scope_traffic_light_test_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(traffic_light_test_cvp)
  { }


  class traffic_light_test_cvp_coverage : public covergroup {
  public :
    CG_SCOPED_CONS(traffic_light_test_cvp_coverage, scope_traffic_light_test_cvp) { }


    int north_light = RED;
    int south_light = RED;
    int east_light  = RED;

    COVERPOINT(int, cvp_north, north_light, true) {
        bin<int>("red",    RED),
        bin<int>("yellow", YELLOW),
        bin<int>("green",  GREEN),
        bin<int>("fault signal", 3)
    };
    COVERPOINT(int, cvp_south, south_light, true) {
        bin<int>("red",    RED),
        bin<int>("yellow", YELLOW),
        bin<int>("green",  GREEN)
    };
    COVERPOINT(int, cvp_east, east_light, true) {
        bin<int>("red",     RED),
        bin<int>("yellow",  YELLOW),
        bin<int>("green",   GREEN)
    };


    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4;
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

      print_coverage(cvp_north);
      print_coverage(cvp_south);
      print_coverage(cvp_east);

      std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;
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

  }; //end of class traffic_light_test_cvp_coverage

  traffic_light_test_cvp_coverage traffic_light_test_cvp;
};


void generate_without_cross_test_cvp()
{
  for (uint8_t version = 0; version < 2; version++)
  {
    auto cntxt = fc4sc::global::create_new_context();
    scope_traffic_light_test_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

    std::string cov_file_name;

    if (version == 0)
    {
      cov_file_name = "without_cross_test_cov_scope_a.xml";
    }
    else
    {
      cov_file_name = "without_cross_test_cov_scope_b.xml";
    }

    std::cout << "Generating " << cov_file_name << std::endl;


    mod.traffic_light_test_cvp.sample();

    if (version == 0) { // file a

        mod.traffic_light_test_cvp.north_light = GREEN;
        mod.traffic_light_test_cvp.south_light = GREEN;
        mod.traffic_light_test_cvp.east_light  = RED;
        mod.traffic_light_test_cvp.sample();
    }
    else { // file b
        mod.traffic_light_test_cvp.north_light = RED;
        mod.traffic_light_test_cvp.south_light = RED;
        mod.traffic_light_test_cvp.east_light  = GREEN;
        mod.traffic_light_test_cvp.sample();
    }

    mod.traffic_light_test_cvp.stop();

    mod.traffic_light_test_cvp.print_coverage();

    xml_printer::coverage_save(cov_file_name, cntxt);

    fc4sc::global::delete_context(cntxt);
  }
}
