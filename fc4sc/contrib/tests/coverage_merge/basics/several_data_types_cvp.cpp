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
#include "several_data_types_cvp.h"
#include "fc4sc.hpp"


class scope_several_data_types_cvp : public fc4sc::scope {

public:
  SCOPE_DECL(scope_several_data_types_cvp)

  scope_several_data_types_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(several_data_types_cvp)
  { }

  class several_data_types_cvp_coverage : public covergroup {
  public :
    CG_SCOPED_CONS(several_data_types_cvp_coverage, scope_several_data_types_cvp) { }


    bool         var_bool = false;
    char         var_char = 'X';
    int          var_int  = 0;
    unsigned int var_uint = 33;
    long         var_long = 33;
    uint16_t     var_uint16 = 0;

    COVERPOINT(bool, cvp_bool,  var_bool, true)  {
        bin<bool> ("false", 0),
        bin<bool> ("true", 1)
    };

    COVERPOINT(char, cvp_char,  var_char, true){
        bin<char>("X", 'X'),
        bin<char>("Y", 'Y')
    };
    COVERPOINT(unsigned int, cvp_uint,  var_uint, true){
        bin<unsigned int>("33", 33),
        bin<unsigned int>("100", 100)
    };
    COVERPOINT(long, cvp_long,  var_long, true) {
        bin<long>("33", 33),
        bin<long>("100", 100)
    };

    COVERPOINT(int, interval__int, var_int, true) {
      ignore_bin<int>("ii_int",     interval(-1000, -1)),
      bin<int> ("small", interval(0,5), interval(7,7), interval(8,20) ),
      bin_array<int> ("larger", 3, interval(21, 100) )
    };

    COVERPOINT(uint16_t, interval_uint16, var_uint16, true) {
        bin<        uint16_t>("small",         interval(0, 100)),
        bin<        uint16_t>("larger",        interval(101, 10000)),
        bin_array<  uint16_t>("upper_values",  3, interval(10001, 65534)),
        ignore_bin< uint16_t>("ii",          65534),
        illegal_bin<uint16_t>("illegal mode", 65535)
    };

    cross < bool , char, unsigned int, long, int, uint16_t> test_cr =
    cross < bool , char, unsigned int, long, int, uint16_t>
    ( this , " test_cr " , &cvp_bool, &cvp_char, &cvp_uint , &cvp_long, &interval__int, &interval_uint16 );


    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;

      print_coverage(cvp_bool);
      print_coverage(cvp_char);
      print_coverage(cvp_uint);
      print_coverage(cvp_long);
      print_coverage(interval__int);
      print_coverage(interval_uint16);
      print_coverage(test_cr);

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

  }; //end of class several_data_types_cvp_coverage

  several_data_types_cvp_coverage  several_data_types_cvp;
};


void generate_several_data_types_cvp()
{

  for (uint8_t version = 0; version < 2; version++)
  {
    auto cntxt = fc4sc::global::create_new_context();
    scope_several_data_types_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

    std::string cov_file_name;

    if (version == 0)
    {
      cov_file_name = "several_data_types_cov_scope_a.xml";
    }
    else
    {
      cov_file_name = "several_data_types_cov_scope_b.xml";
    }

    std::cout << "Generating " << cov_file_name << std::endl;


    //mod.several_data_types_cvp.var_uint16 = 65535; //illegal_mode
    //mod.several_data_types_cvp.sample();

    if (version == 0) { //file a
        mod.several_data_types_cvp.var_char = 'Y';
        mod.several_data_types_cvp.var_bool = true;
        mod.several_data_types_cvp.var_long = 100;
        mod.several_data_types_cvp.var_int = 99;
        mod.several_data_types_cvp.var_uint16 = 10001;

        mod.several_data_types_cvp.sample();
        mod.several_data_types_cvp.sample();
        mod.several_data_types_cvp.sample();

        mod.several_data_types_cvp.var_uint16 = 101;
        mod.several_data_types_cvp.sample();
    }
    else { //file b
        mod.several_data_types_cvp.var_int = -1; //ignore bin
        mod.several_data_types_cvp.var_uint16 = 65534; //ignore bin
        mod.several_data_types_cvp.sample();

        mod.several_data_types_cvp.var_char = 'Y';
        mod.several_data_types_cvp.var_int = -999; //ignore bin
        mod.several_data_types_cvp.var_uint16 = 55;
        mod.several_data_types_cvp.sample();
    }

    mod.several_data_types_cvp.stop();

    mod.several_data_types_cvp.print_coverage();

    xml_printer::coverage_save( cov_file_name, cntxt);

    fc4sc::global::delete_context(cntxt);
  }
}
