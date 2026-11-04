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
#include "uint8_cvp.h"

class uint8_cvp_coverage : public covergroup
{
public :
  CG_CONS(uint8_cvp_coverage)
  { }

  uint8_t  mode   {0};

  COVERPOINT( uint8_t, test_cvp,  mode, true)
  {
    bin<uint8_t>("zero", 0),
    bin<uint8_t>("one",  1),
    bin<uint8_t>("two",  2)
  };

  COVERPOINT( uint8_t, interval_cvp,  mode, true)
  {
    bin<        uint8_t>("zero",           0),
    bin<        uint8_t>("small",         interval(1, 3)),
    bin<        uint8_t>("larger",         4, interval(5, 10)),
    bin<        uint8_t>("larger2",       11, 12, interval(13, 15), 16),
    bin_array<  uint8_t>("upper_values",   3, interval(17, 55)),
    ignore_bin< uint8_t>("ii",            77),
    illegal_bin<uint8_t>("illegal mode", 255)
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

  cross<uint8_t, uint8_t> cvp1_cvp2_cr     = cross<uint8_t, uint8_t>(this, "cvp1_cvp2_cr",     &cvp1,     &cvp2);
  cross<uint8_t, uint8_t> test_interval_cr = cross<uint8_t, uint8_t>(this, "test_interval_cr", &test_cvp, &interval_cvp);
  cross<uint8_t, uint8_t, uint8_t, uint8_t> four_cr = cross<uint8_t, uint8_t, uint8_t, uint8_t>(this, "four_cr", &cvp1, &cvp2, &test_cvp, &interval_cvp);

  void print_coverage() const
  {
    const int prec = 3;
    const int width = prec + 4; // 3 digits for "100" and the point
    std::cout << std::endl << std::setprecision(prec) << std::fixed;
    std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

    print_coverage(test_cvp);
    print_coverage(interval_cvp);
    print_coverage(cvp1);
    print_coverage(cvp2);
    print_coverage(cvp1_cvp2_cr);
    print_coverage(test_interval_cr);
    print_coverage(four_cr);
  }

  template <typename... T1>
  void print_coverage(cross<T1...> cr) const
  {
    std::cout << std::endl << std::setw(7) << cr.get_inst_coverage() << " % " << cr.name() << std::endl;
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
    std::cout << std::endl << std::setprecision(3) << std::fixed;
    std::cout << std::setw(7) << cvp.get_inst_coverage() << " % " << cvp.name() << std::endl;
    for (uint64_t i = 0; i < cvp.size(); i++)
    {
      std::cout << "( " << std::setw(4) << i << " ) : " << std::setw(4) << cvp.get_bin_hit_count(i)  << std::endl;
    }
  }

};

void generate_uint8_cvp()
{
  uint8_cvp_coverage uint8_cvp("uint8_cvp_coverage");
  std::string cov_file_name("uint8_cvp_coverage");

  std::cout << std::endl << "Generating " << cov_file_name << std::endl;

  uint8_cvp.mode = 0;
  uint8_cvp.sample();

  uint8_cvp.mode = 2;
  uint8_cvp.sample();

  uint8_cvp.mode = 6;
  uint8_cvp.sample();

  uint8_cvp.mode = 12;
  uint8_cvp.sample();
  uint8_cvp.sample();
  uint8_cvp.sample();

  uint8_cvp.v1 = 1;
  uint8_cvp.v2 = 2;
  uint8_cvp.c1 = true;
  uint8_cvp.c2 = true;
  uint8_cvp.sample();

  uint8_cvp.v1 = 1;
  uint8_cvp.v2 = 0;
  uint8_cvp.c1 = false;
  uint8_cvp.c2 = true;
  uint8_cvp.sample();

  uint8_cvp.v1 = 2;
  uint8_cvp.v2 = 1;
  uint8_cvp.c1 = true;
  uint8_cvp.c2 = false;
  uint8_cvp.sample();

  uint8_cvp.v1 = 0;
  uint8_cvp.v2 = 0;
  uint8_cvp.c1 = false;
  uint8_cvp.c2 = false;
  uint8_cvp.sample();

  uint8_cvp.v1 = 2;
  uint8_cvp.v2 = 2;
  uint8_cvp.c1 = true;
  uint8_cvp.c2 = true;
  uint8_cvp.sample();

  uint8_cvp.stop();

  uint8_cvp.print_coverage();

  xml_printer::coverage_save(cov_file_name + ".xml");
}
