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
   Authors  : Valentin Siegel,          HTW Dresden, 2026-11-03
              Jens Schoenherr,          HTW Dresden, 2026-11-03
******************************************************************************/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>
#include "fc4sc.hpp"
#include "html_printer.hpp"
#include "uint8_cvp.h"

class scope_uint8 : public fc4sc::scope
{

public:
  SCOPE_DECL(scope_uint8)

  scope_uint8(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(uint8_cvg)
  { }

  class uint8_cvp_coverage : public covergroup
  {
  public :

    CG_SCOPED_CONS(uint8_cvp_coverage, scope_uint8) { }

    uint8_t  mode   {0};

    COVERPOINT( uint8_t, test_cvp1,  mode, true)
    {
      bin<uint8_t>("zero", 0),
      bin<uint8_t>("one",  1),
      bin<uint8_t>("two",  2),
      bin<uint8_t>("three",  3)
    };


    COVERPOINT( uint8_t, test_cvp2,  mode, true)
    {
      bin<uint8_t>("four", 4),
      bin<uint8_t>("five",  interval(5,9)),
      bin<uint8_t>("six",  interval(7,12)),
      bin<uint8_t>("seven",  12),
      bin<uint8_t>("eight",  12)
    };

    COVERPOINT( uint8_t, test_cvp3, mode, true) {
      ignore_bin<uint8_t>("zero", 0) // at least one bin is required
    };

    COVERPOINT( uint8_t, test_cvp4, mode, true) {
      ignore_bin<uint8_t>("zero", 0) // at least one bin is required
    };

    COVERPOINT( uint8_t, interval_cvp,  mode, true)
    {
      bin<        uint8_t>("zero",           0),
      bin<        uint8_t>("small",         interval(1, 3)),
      bin<        uint8_t>("larger",         4, interval(5, 10)),
      bin<        uint8_t>("larger2",       10, 11, interval(12, 15), interval(18,21), 22),
      bin_array<  uint8_t>("upper_values",   3, interval(17, 55)),
      ignore_bin< uint8_t>("ii",            77),
      illegal_bin<uint8_t>("illegal mode", 255)
    };

    COVERPOINT( uint8_t, array_cvp, mode, true)
    {
      bin_array<uint8_t>(" 9/3", 3, interval( 0,  8)),
      bin_array<uint8_t>("10/3", 3, interval(10, 19)),
      bin_array<uint8_t>("11/3", 3, interval(20, 30))
    };

    bool c1 {false};
    bool c2 {false};
    uint8_t v1 {0};
    uint8_t v2 {0};
    COVERPOINT( uint8_t, cvp1, v1, c1)
    {
      bin<uint8_t>("zero", 0),
      bin<uint8_t>("one",  1),
      bin<uint8_t>("two",  2)
    };
    COVERPOINT( uint8_t, cvp2, v2, c2)
    {
      bin<uint8_t>("zero", 0),
      bin<uint8_t>("one",  1),
      bin<uint8_t>("two",  2)
    };

    cross<uint8_t, uint8_t> cvp1_cvp2_cr     = cross<uint8_t, uint8_t>(this, "cvp1_cvp2_cr",     &cvp1, &cvp2);
    cross<uint8_t, uint8_t> test_interval_cr = cross<uint8_t, uint8_t>(this, "test_interval_cr", &test_cvp1, &interval_cvp);
    cross<uint8_t, uint8_t, uint8_t, uint8_t> four_cr = cross<uint8_t, uint8_t, uint8_t, uint8_t>(this, "four_cr", &cvp1, &cvp2, &test_cvp1, &interval_cvp);

    cross<uint8_t, uint8_t> cvp3_cvp4_empty_cr = cross<uint8_t, uint8_t>(this, "cvp3_cvp4_empty_cr", &test_cvp3, &test_cvp4);


    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

      print_coverage(test_cvp1);
      print_coverage(test_cvp2);
      print_coverage(test_cvp3);
      print_coverage(test_cvp4);
      print_coverage(interval_cvp);
      print_coverage(array_cvp);
      print_coverage(cvp1);
      print_coverage(cvp2);
      print_coverage(cvp1_cvp2_cr);
      print_coverage(test_interval_cr);
      print_coverage(four_cr);
      print_coverage(cvp3_cvp4_empty_cr);

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

  }; // end of class uint8_cvp_coverage

  uint8_cvp_coverage  uint8_cvg;
};

void generate_uint8_cvp()
{
  auto cntxt = fc4sc::global::create_new_context();
  scope_uint8 mod("scope_uint8", __FILE__, __LINE__, cntxt);

  std::string cov_file_name("uint8_cvp_coverage");
  std::string xml(".xml");
  std::string html(".html");

  std::cout << "Generating " << cov_file_name << std::endl;

  mod.uint8_cvg.mode = 0;
  mod.uint8_cvg.sample();

  mod.uint8_cvg.mode = 2;
  mod.uint8_cvg.sample();

  mod.uint8_cvg.mode = 6;
  mod.uint8_cvg.sample();

  mod.uint8_cvg.mode = 12;
  mod.uint8_cvg.sample();
  mod.uint8_cvg.sample();
  mod.uint8_cvg.sample();

  //mod.uint8_cvg.mode = 255;
  //mod.uint8_cvg.sample();

  mod.uint8_cvg.v1 = 1;
  mod.uint8_cvg.v2 = 2;
  mod.uint8_cvg.c1 = true;
  mod.uint8_cvg.c2 = true;
  mod.uint8_cvg.sample();

  mod.uint8_cvg.v1 = 1;
  mod.uint8_cvg.v2 = 0;
  mod.uint8_cvg.c1 = false;
  mod.uint8_cvg.c2 = true;
  mod.uint8_cvg.sample();

  mod.uint8_cvg.v1 = 2;
  mod.uint8_cvg.v2 = 1;
  mod.uint8_cvg.c1 = true;
  mod.uint8_cvg.c2 = false;
  mod.uint8_cvg.sample();

  mod.uint8_cvg.v1 = 0;
  mod.uint8_cvg.v2 = 0;
  mod.uint8_cvg.c1 = false;
  mod.uint8_cvg.c2 = false;
  mod.uint8_cvg.sample();

  mod.uint8_cvg.v1 = 2;
  mod.uint8_cvg.v2 = 2;
  mod.uint8_cvg.c1 = true;
  mod.uint8_cvg.c2 = true;
  mod.uint8_cvg.sample();


  mod.uint8_cvg.stop();

  mod.uint8_cvg.print_coverage();

  xml_printer::coverage_save(cov_file_name + xml, cntxt);
  html_printer::coverage_save(cov_file_name + "_full_2"    + html, cntxt, html_printer::table_format::full,    2);
  html_printer::coverage_save(cov_file_name + "_partial_3" + html, cntxt, html_printer::table_format::partial, 3);
  html_printer::coverage_save(cov_file_name + "_compact_1" + html, cntxt, html_printer::table_format::compact, 1);

  fc4sc::global::delete_context(cntxt);
}
