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
#include "bins.h"

class scope_minimal : public fc4sc::scope
{
public:
  SCOPE_DECL(scope_minimal)

  scope_minimal(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt)
    { }

};

class bin_test_cg : public covergroup
{
public :

  CG_SCOPED_CONS(bin_test_cg, scope_minimal) { }

  uint8_t  abc   {0};

  // create a vector of values
  std::vector<uint8_t> even_numbers(uint8_t max)
  {
    std::vector<uint8_t> result;
    for (uint8_t i = 2; i <= max; i+=2)
    {
      result.push_back(i);
    }
    return result;
  }

  COVERPOINT( uint8_t, ba1_cp, abc, true) {
    bin<      uint8_t>("zero", 0),
    bin<      uint8_t>("one",  1),
    bin_array<uint8_t>("even", even_numbers(10))
  };

  // create a vector of intervals
  std::vector< fc4sc::interval_t<uint8_t> > log_ranges(uint8_t start, int8_t num)
  {
    std::vector< fc4sc::interval_t<uint8_t> > result;
    uint8_t a = start == 0 ? 0 : 1<<start;
    uint8_t b = start == 0 ? 1 : (a*2)-1;
    for (uint8_t i = 0; i < num; i++)
    {
      fc4sc::interval_t<uint8_t> intrvl = interval(a, b);
      result.push_back(intrvl);
      a = b+1;
      b = (2*b) + 1;
    }
    return result;
  }

  COVERPOINT( uint8_t, bin2_cp,  abc, true)
  {
    bin<uint8_t>("many", log_ranges(0, 4), log_ranges(5, 2), 255)
  };

  COVERPOINT( uint8_t, ba2_cp, abc, true)
  {
    bin_array<uint8_t>("log", log_ranges(0, 4))
  };

  // create a value
  uint8_t succ(uint8_t n)
  {
    return static_cast<uint8_t>(n+1);
  }

  // create an interval
  fc4sc::interval_t<uint8_t> pred_succ(uint8_t n)
  {
    return interval(static_cast<uint8_t>(n-1), static_cast<uint8_t>(n+1));
  }

  COVERPOINT( uint8_t, bin3_cp,  abc, true)
  {
    bin<uint8_t>("succ", succ(2), succ(7)),
    bin<uint8_t>("pair", pred_succ(10), pred_succ(20)),
    bin<uint8_t>("mix",  pred_succ(100), succ(20), 123, interval(200, 204))
  };

  void print_coverage() const
  {
    const int prec = 3;
    const int width = prec + 4; // 3 digits for "100" and the point
    std::cout << std::endl << std::setprecision(prec) << std::fixed;
    std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

    print_coverage(bin2_cp);
    print_coverage(bin3_cp);
    print_coverage(ba1_cp);
    print_coverage(ba2_cp);
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

void test_bins()
{
  fc4sc::global *cntxt = fc4sc::global::create_new_context();

  scope_minimal mod1("mod1", __FILE__, __LINE__, cntxt);

  bin_test_cg cg1(mod1, "cg1", __FILE__, __LINE__);

  cg1.abc = 1;
  cg1.sample();

  cg1.stop();

  cg1.print_coverage();

  xml_printer::coverage_save("bins.xml", cntxt);

  fc4sc::global::delete_context(cntxt);
}
