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
#include "char_cvp.h"

class char_cvp_coverage : public covergroup {
public :
  CG_CONS(char_cvp_coverage) {
  }

  char  mode   {'a'};

  COVERPOINT( char, test_cvp,  mode, true) {
    bin<char>("char_a", 'a'),
    bin<char>("char_b", 'b'),
    bin<char>("char_c", 'c'),
    bin<char>("char_d", 'd')
    };

  COVERPOINT( char, interval_cvp,  mode, true) {
    bin<        char>("zero",         'a'),
    bin<        char>("newBin",       'b'),
    bin<        char>("small",         interval('c', 'e')),
    bin<        char>("larger",       'f', interval('g', 'l')),
    bin<        char>("larger2",      'm', 'n', interval('o', 'q'), 'r'),
    bin_array<  char>("upper_values", 's', interval('u', 'w')),
    ignore_bin< char>("ii",           'x'),
    illegal_bin<char>("illegal mode", 'z')
  };

  bool c1 {false};
  bool c2 {false};
  char v1 {0};
  char v2 {0};

  COVERPOINT( char, cvp1, v1, c1) {
    bin<char>("zero", 'a'),
    bin<char>("one",  'b'),
    bin<char>("two",  'c')
  };

  COVERPOINT( uint8_t, cvp2, v2, c2) {
    bin<uint8_t>("zero", 0),
    bin<uint8_t>("one",  1),
    bin<uint8_t>("two",  2)
  };

  cross<char, uint8_t> cvp1_cvp2_cr      = cross<char, uint8_t>(this, "cvp1_cvp2_cr",      &cvp1,     &cvp2);
  cross<char, char> test_interval_cr = cross<char, char>(this, "test_interval_cr", &test_cvp, &interval_cvp);

  void print_coverage() const
  {
    const int prec = 3;
    const int width = prec + 4; // 3 digits for "100" and the point
    std::cout << std::setprecision(prec) << std::fixed;
    std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

    print_coverage(test_cvp);
    print_coverage(interval_cvp);
    print_coverage(cvp1);
    print_coverage(cvp2);

    print_coverage(cvp1_cvp2_cr);
    print_coverage(test_interval_cr);

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

};

void generate_char_cvp()
{

  char_cvp_coverage char_cvp("char_cvp_coverage");
  std::string cov_file_name("char_cvp_coverage");
  std::string xml(".xml");
  std::string html(".html");

  std::cout << "Generating " << cov_file_name << std::endl;

  char_cvp.mode = 'a';
  char_cvp.sample();
  char_cvp.sample();
  char_cvp.sample();
  char_cvp.sample();
  char_cvp.sample();

  char_cvp.mode = 'b';
  char_cvp.sample();

  char_cvp.mode = 'g';
  char_cvp.sample();

  char_cvp.mode = 'g';
  char_cvp.sample();
  char_cvp.sample();
  char_cvp.sample();

  char_cvp.mode = 'x';
  char_cvp.sample();

  char_cvp.v1 = 1;
  char_cvp.v2 = 2;
  char_cvp.c1 = true;
  char_cvp.c2 = true;
  char_cvp.sample();

  char_cvp.v1 = 1;
  char_cvp.v2 = 0;
  char_cvp.c1 = false;
  char_cvp.c2 = true;
  char_cvp.sample();

  char_cvp.v1 = 2;
  char_cvp.v2 = 1;
  char_cvp.c1 = true;
  char_cvp.c2 = false;
  char_cvp.sample();

  char_cvp.v1 = 0;
  char_cvp.v2 = 0;
  char_cvp.c1 = false;
  char_cvp.c2 = false;
  char_cvp.sample();

  char_cvp.v1 = 2;
  char_cvp.v2 = 2;
  char_cvp.c1 = true;
  char_cvp.c2 = true;
  char_cvp.sample();

  char_cvp.stop();

  char_cvp.print_coverage();

  xml_printer::coverage_save(cov_file_name + xml);
  html_printer::coverage_save(cov_file_name + "_full_1"    + html, fc4sc::global::getter(), html_printer::table_format::full,    1);
  html_printer::coverage_save(cov_file_name + "_partial_2" + html, fc4sc::global::getter(), html_printer::table_format::partial, 2);
  html_printer::coverage_save(cov_file_name + "_compact_3" + html, fc4sc::global::getter(), html_printer::table_format::compact, 3);

}
