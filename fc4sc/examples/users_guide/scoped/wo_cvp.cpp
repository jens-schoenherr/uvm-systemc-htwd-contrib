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
#include "wo_cvp.h"

class scope_wo_cvp : public fc4sc::scope
{

public:
  SCOPE_DECL(scope_wo_cvp)

  scope_wo_cvp(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(wo_cvp)
  { }

  class wo_cvp_coverage : public covergroup
  {
  public :
    CG_SCOPED_CONS(wo_cvp_coverage, scope_wo_cvp) { }

    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::endl << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete coverpoint)" << std::endl;
    }

  };

  wo_cvp_coverage  wo_cvp;
};


void generate_wo_cvp()
{
  auto cntxt = fc4sc::global::create_new_context();
  scope_wo_cvp mod("default_scope_module", __FILE__, __LINE__, cntxt);

  std::string cov_file_name("wo_cvp_coverage");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  mod.wo_cvp.sample();
  mod.wo_cvp.stop();

  mod.wo_cvp.print_coverage();

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  fc4sc::global::delete_context(cntxt);
}
