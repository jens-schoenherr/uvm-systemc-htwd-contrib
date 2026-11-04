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
#include "hierarchical_scope.h"
#include "context_info.h"

class top_scope : public fc4sc::scope
{

public:
  SCOPE_DECL(top_scope)

  top_scope(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt)
  { }

};

class sub1_scope : public fc4sc::scope
{

public:
  SCOPE_DECL(sub1_scope, top_scope)

  sub1_scope(fc4sc::scp_base& p_mod, std::string name_, const char *auto_inst_file_name = "", int auto_line = 1) :
    fc4sc::scope(p_mod, name_, __FILE__, __LINE__, auto_inst_file_name, auto_line)
  { }

};

class sub2_scope : public fc4sc::scope
{

public:
  SCOPE_DECL(sub2_scope, top_scope)

  sub2_scope(fc4sc::scp_base& p_mod, std::string name_, const char *auto_inst_file_name = "", int auto_line = 1) :
    fc4sc::scope(p_mod, name_, __FILE__, __LINE__, auto_inst_file_name, auto_line)
  { }

};

class subsub_scope : public fc4sc::scope
{

public:
  SCOPE_DECL(subsub_scope, sub1_scope)

  subsub_scope(fc4sc::scp_base& p_mod, std::string name_, const char *auto_inst_file_name = "", int auto_line = 1) :
    fc4sc::scope(p_mod, name_, __FILE__, __LINE__, auto_inst_file_name, auto_line)
  { }

};


class cg_top_scoped : public covergroup
{
public :

  using covergroup::sample;
  cg_top_scoped(fc4sc::scp_base& scp, std::string inst_name = "", const char *auto_inst_file_name = "", int auto_line = 1) :
    fc4sc::covergroup(scp, "cg_top_scoped", scp.type_name(), __FILE__, __LINE__, inst_name, auto_inst_file_name, auto_line) { }

  uint8_t  mode   {0};

  COVERPOINT( uint8_t, mode_cvp,  mode, true)
  {
    bin<uint8_t>("zero", 0),
    bin<uint8_t>("one",  1),
    bin<uint8_t>("two",  2)
  };

};

class cg_sub_scoped : public covergroup
{
public :

  using covergroup::sample;
  cg_sub_scoped(fc4sc::scp_base& scp, std::string inst_name = "", const char *auto_inst_file_name = "", int auto_line = 1) :
    fc4sc::covergroup(scp, "cg_sub_scoped", scp.type_name(), __FILE__, __LINE__, inst_name, auto_inst_file_name, auto_line) { }

  uint8_t  mode   {0};

  COVERPOINT( uint8_t, mode_cvp,  mode, true)
  {
    bin<uint8_t>("zero", 0),
    bin<uint8_t>("one",  1),
    bin<uint8_t>("two",  2)
  };

};


void generate_hierarchical_scope()
{
  std::string cov_file_name("hierarchical_scope");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  fc4sc::global *cntxt = fc4sc::global::create_new_context();

  top_scope    top_scp1(              "top_scp1",    __FILE__, __LINE__, cntxt);
  sub1_scope   sub1_scp1(  top_scp1,  "sub1_scp1",   __FILE__, __LINE__);
  sub2_scope   sub2_scp1(  top_scp1,  "sub2_scp1",   __FILE__, __LINE__);
  sub2_scope   sub2_scp2(  top_scp1,  "sub2_scp2",   __FILE__, __LINE__);
  subsub_scope subsub_scp1(sub1_scp1, "subsub_scp1", __FILE__, __LINE__);

  cg_top_scoped cg_t1(top_scp1,  "cg_t1", __FILE__, __LINE__);
  cg_top_scoped cg_s1(sub1_scp1, "cg_s1", __FILE__, __LINE__);

  std::cout << top_scope::scp_type_name()    << std::endl; // --> top_scope
  std::cout << sub1_scope::scp_type_name()   << std::endl; // --> top_scope::sub1_scope
  std::cout << sub2_scope::scp_type_name()   << std::endl; // --> top_scope::sub2_scope
  std::cout << subsub_scope::scp_type_name() << std::endl; // --> top_scope::sub1_scope::subsub_scope

  std::cout << std::endl;

  std::cout << top_scp1.type_name() << std::endl;    // --> top_scope
  std::cout << top_scp1.name() << std::endl;         // --> top_scp1
  std::cout << sub1_scp1.type_name() << std::endl;   // --> top_scope::sub1_scope
  std::cout << sub1_scp1.name() << std::endl;        // --> sub1_scp1
  std::cout << sub2_scp1.type_name() << std::endl;   // --> top_scope::sub2_scope
  std::cout << sub2_scp1.name() << std::endl;        // --> sub2_scp2
  std::cout << sub2_scp2.type_name() << std::endl;   // --> top_scope::sub2_scope
  std::cout << sub2_scp2.name() << std::endl;        // --> sub2_scp1
  std::cout << subsub_scp1.type_name() << std::endl; // --> top_scope::sub1_scope::subsub_scope
  std::cout << subsub_scp1.name() << std::endl;      // --> subsub_scp1
  {
    std::string s0 = top_scope::scp_type_name();
    std::string s1 = top_scp1.type_name();
    std::string s2 = top_scp1.name();
  }

  std::cout << std::endl;

  std::cout << cg_t1.name() << std::endl;           // cg_t1
  std::cout << cg_t1.scp_type_name() << std::endl;  // top_scope
  std::cout << cg_t1.get_cvg_data()->parent_scp->name << std::endl; // top_scp1

  std::cout << cg_s1.name() << std::endl;           // cg_s1
  std::cout << cg_s1.scp_type_name() << std::endl;  // sub_scope
  std::cout << cg_s1.get_cvg_data()->parent_scp->name << std::endl; // sub_scp1

  std::cout << "sub scope types" << std::endl;
  for (auto sub_scope_type : top_scp1.get_scp_data()->child_scps)
  {
    std::cout << "  " << sub_scope_type.first << std::endl;
  }
  std::cout << "sub scope instances" << std::endl;
  for (auto sub_scope_inst : top_scp1.child_scp_insts)
  {
    std::cout << "  " << sub_scope_inst.first << std::endl;
  }
  std::cout << "sub scope types" << std::endl;
  for (auto sub_scope_type : sub1_scp1.get_scp_data()->child_scps)
  {
    std::cout << "  " << sub_scope_type.first << std::endl;
  }
  std::cout << "sub scope instances" << std::endl;
  for (auto sub_scope_inst : sub1_scp1.child_scp_insts)
  {
    std::cout << "  " << sub_scope_inst.first << std::endl;
  }

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  print_context_info(cntxt);

  fc4sc::global::delete_context(cntxt);
}
