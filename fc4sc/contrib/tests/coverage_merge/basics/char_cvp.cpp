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
#include "char_cvp.h"
#include "fc4sc.hpp"


class scope_char_cvp : public fc4sc::scope {

public:
  SCOPE_DECL(scope_char_cvp)

  scope_char_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(char_cvp)
  { }

  class char_cvp_coverage : public covergroup {
  public :
    CG_SCOPED_CONS(char_cvp_coverage, scope_char_cvp) { }

    char mode_char {'A'};
    char mode_char_new {'a'};
    char mode_char_intervall {'0'};
    char v1_char {'X'};
    char v2_char {'Y'};
    bool c1 {true};
    bool c2 {true};

    COVERPOINT(char, test_cvp, mode_char, true) {
        bin<char>("char_A", 'A'),
        bin<char>("char_B", 'B'),
        bin<char>("char_C", 'C')
    };

    COVERPOINT(char, test_cvp_new, mode_char_new, true) {
        bin<char>("char_a", 'a'),
        bin<char>("char_b", 'b'),
        bin<char>("char_c", 'c')
    };

    // Intervall (ASCII)
    COVERPOINT(char, intervall_cvp, mode_char_intervall, true) {
        bin<char>("digits", interval('0', '9')),   //0-9             // ASCII [48-57]
        bin_array<char>("lowercase_letters ([1]a-i, [2]j-r, [3]s-z) - ", 3, interval('a', 'z')), // a-z (3 bins)
        ignore_bin<char>("ignore_space", ' '),                       // ignore bin
        illegal_bin<char>("illegal_null", '\0')                      // illegal bin
    };

    COVERPOINT(char, cvp1, v1_char, c1) {
        bin<char>("val_X", 'X'),
        bin<char>("val_Y", 'Y')
    };

    COVERPOINT(char, cvp2, v2_char, c2) {
        bin<char>("val_X", 'X'),
        bin<char>("val_Y", 'Y')
    };

    // cross with 3 elements (Cross Coverage)
    cross<char, char> cvp1_cvp2_cr = cross<char, char>(this, "cvp1_cvp2_cr", &cvp1, &cvp2);
    cross<char, char, char> test_intervall_cr = cross<char, char, char>(this, "test_intervall_cr", &test_cvp, &test_cvp_new, &intervall_cvp);


    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;

      print_coverage(test_cvp);
      print_coverage(test_cvp_new);
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

  }; //end of class char_cvp_coverage

  char_cvp_coverage  char_cvp;
};


void generate_char_cvp()
{

  for (uint8_t version = 0; version < 2; version++)
  {
    auto cntxt = fc4sc::global::create_new_context();
    scope_char_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

    std::string cov_file_name;

    if (version == 0)
    {
      cov_file_name = "char_cov_scope_a.xml";
    }
    else
    {
      cov_file_name = "char_cov_scope_b.xml";
    }

    std::cout << "Generating " << cov_file_name << std::endl;

    //test:
     // file a
    if (version == 0) {
        mod.char_cvp.v1_char = 'X'; mod.char_cvp.v2_char = 'X';
        mod.char_cvp.mode_char_intervall = ' '; //test ignore bin
        mod.char_cvp.sample(); //cvp2 x cvp1 {0,0} ({X,X})

        mod.char_cvp.mode_char = 'A';
        mod.char_cvp.mode_char_new = 'b';
        mod.char_cvp.mode_char_intervall = 'v'; //intervall_cvp x test_cvp_new x test_cvp {3,1,0}

        mod.char_cvp.v1_char = 'Y'; mod.char_cvp.v2_char = 'Y';
        mod.char_cvp.sample(); //cvp2 x cvp1 {1,1} ({Y,Y})
    }
    // file b
    else {
        mod.char_cvp.v1_char = 'X'; mod.char_cvp.v2_char = 'Y';
        mod.char_cvp.sample(); //cvp2 x cvp1 {1,0} ({Y,X})

        mod.char_cvp.mode_char = 'B';
        mod.char_cvp.mode_char_new = 'c'; //intervall_cvp x test_cvp_new x test_cvp {0,2,1}

        mod.char_cvp.v1_char = 'Y'; mod.char_cvp.v2_char = 'X';
        mod.char_cvp.sample(); //cvp2 x cvp1 {0,1} ({X,Y})
    }


    mod.char_cvp.stop();

    mod.char_cvp.print_coverage();

    xml_printer::coverage_save( cov_file_name, cntxt);

    fc4sc::global::delete_context(cntxt);
  }
}
