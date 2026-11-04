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
#include "dynamic_scope.h"
#include "context_info.h"

void try_dynamic_covergroup()
{
  std::string cov_file_name("dynamic_coverpoint");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  fc4sc::dynamic_covergroup_factory cvg("cvg");

  fc4sc::dynamic_coverpoint_factory<int(int, int), bool(int)>
    cvp_sum = cvg.create_coverpoint< int(int, int), // sampling function
                                     bool(int) >    // condition function
                                   ( "sum",                          // name
                                     [](int x, int y) {return x+y;}, // sampling function
                                     [](int x) {return x == -1;} );  // condition function
  cvp_sum.create_bin("ZERO", 0);
  cvp_sum.create_bin("ONE",  1);

  fc4sc::dynamic_coverpoint_factory<int(int, int), bool()>
  cvp_diff = cvg.create_coverpoint< int(int, int) >
                                  ("diff", [](int x, int y) {return x-y;} );

  cvp_diff.create_bin("ZERO", 0);
  cvp_diff.create_bin("TWO",  2);

  cvg.add_cross("cr", cvp_sum, cvp_diff);

  fc4sc::global *cntxt = fc4sc::global::create_new_context();
  fc4sc::dynamic_covergroup cvg1(cvg, "cvg1", __FILE__, __LINE__, cntxt);

  int v1, v2; // variables to be sampled

  cvp_sum.bind_sample(   cvg1, v1, v2);
  cvp_sum.bind_condition(cvg1, v2);

  cvp_diff.bind_sample(cvg1, v1, v2);

  // binding complete, usage can start now
  v1 =  1;
  v2 = -1;

  cvg1.sample();

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  print_context_info(cntxt);

  fc4sc::global::delete_context(cntxt);
}

void try_dynamic_scope()
{
  std::string cov_file_name("dynamic_scope");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  fc4sc::global *cntxt = fc4sc::global::create_new_context();

  fc4sc::dynamic_scope_factory      scp("scp");
  fc4sc::dynamic_covergroup_factory cvg(scp, "cvg");

  fc4sc::dynamic_coverpoint_factory<int(int, int), bool(int)>
    cvp_sum = cvg.create_coverpoint< int(int, int), // sampling function
                                     bool(int) >    // condition function
                                   ( "sum",                          // name
                                     [](int x, int y) {return x+y;}, // sampling function
                                     [](int x) {return x == -1;} );  // condition function
  cvp_sum.create_bin("ZERO", 0);
  cvp_sum.create_bin("ONE",  1);

  scp.add_covergroup(cvg, "cvg1"); // instance cvg1 of cvg belongs to scp

  fc4sc::dynamic_scope scp1(scp, "scp1", __FILE__, __LINE__, cntxt); // creates scp1 and cvg1

  fc4sc::dynamic_covergroup &cvg1 = dynamic_cast<fc4sc::dynamic_covergroup&>(scp1.get_covergroup("cvg1"));

  std::cout << cvg1.name() << std::endl;
  std::cout << cvg1.type_name() << std::endl;

  int v1, v2; // variables to be sampled

  cvp_sum.bind_sample(   cvg1, v1, v2);
  cvp_sum.bind_condition(cvg1, v2);

  v1 =  1;
  v2 = -1;

  cvg1.sample();

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  print_context_info(cntxt);

  fc4sc::global::delete_context(cntxt);
}

void  generate_dynamic_scope()
{
  try_dynamic_covergroup();
  try_dynamic_scope();
}
