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
#include "minimal_cvp.h"
#include "context_info.h"

class scope_minimal : public fc4sc::scope
{

public:
  SCOPE_DECL(scope_minimal)

  scope_minimal(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(min_cvg_a1), CG_SCOPED_INST(min_cvg_a2),
    CG_SCOPED_INST(min_cvg_b1), CG_SCOPED_INST(min_cvg_b2)
    { }

  class minimal_scoped_cvp_coverage_a : public covergroup
  {
  public :

    CG_SCOPED_CONS(minimal_scoped_cvp_coverage_a, scope_minimal) { }

    uint8_t  mode   {0};

    COVERPOINT( uint8_t, mode_cvp,  mode, true)
    {
      bin<uint8_t>("zero", 0),
      bin<uint8_t>("one",  1),
      bin<uint8_t>("two",  2)
    };

    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

      print_coverage(mode_cvp);
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

  }; // end of class minimal_scoped_cvp_coverage

  minimal_scoped_cvp_coverage_a  min_cvg_a1;
  minimal_scoped_cvp_coverage_a  min_cvg_a2;

  class minimal_scoped_cvp_coverage_b : public covergroup
  {
  public :

    CG_SCOPED_CONS(minimal_scoped_cvp_coverage_b, scope_minimal) { }

    uint8_t  mode   {0};

    COVERPOINT( uint8_t, mode_cvp,  mode, true)
    {
      bin<uint8_t>("zero", 0),
      bin<uint8_t>("one",  1),
      bin<uint8_t>("two",  2)
    };

    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

      print_coverage(mode_cvp);
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

  }; // end of class minimal_scoped_cvp_coverage_b

  minimal_scoped_cvp_coverage_b  min_cvg_b1;
  minimal_scoped_cvp_coverage_b  min_cvg_b2;
};

class scope_minimal_z : public fc4sc::scope
{

public:
  SCOPE_DECL(scope_minimal_z)

  scope_minimal_z(std::string name_, const char *auto_inst_file_name = "", int auto_line = 1, fc4sc::global* cntxt = fc4sc::global::getter()) :
    fc4sc::scope(name_, __FILE__, __LINE__, auto_inst_file_name, auto_line, cntxt),
    CG_SCOPED_INST(min_cvg_z_c1), CG_SCOPED_INST(min_cvg_z_c2),
    CG_SCOPED_INST(min_cvg_z_d1), CG_SCOPED_INST(min_cvg_z_d2)
    { }

  class minimal_scoped_cvp_coverage_c : public covergroup
  {
  public :

    CG_SCOPED_CONS(minimal_scoped_cvp_coverage_c, scope_minimal_z) { }

    uint8_t  mode   {0};

    COVERPOINT( uint8_t, mode_cvp,  mode, true)
    {
      bin<uint8_t>("zero", 0),
      bin<uint8_t>("one",  1),
      bin<uint8_t>("two",  2)
    };

    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

      print_coverage(mode_cvp);
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

  }; // end of class minimal_scoped_cvp_coverage

  minimal_scoped_cvp_coverage_c  min_cvg_z_c1;
  minimal_scoped_cvp_coverage_c  min_cvg_z_c2;

  class minimal_scoped_cvp_coverage_d : public covergroup
  {
  public :

    CG_SCOPED_CONS(minimal_scoped_cvp_coverage_d, scope_minimal_z) { }

    uint8_t  mode   {0};

    COVERPOINT( uint8_t, mode_cvp,  mode, true)
    {
      bin<uint8_t>("zero", 0),
      bin<uint8_t>("one",  1),
      bin<uint8_t>("two",  2)
    };

    void print_coverage() const
    {
      const int prec = 3;
      const int width = prec + 4; // 3 digits for "100" and the point
      std::cout << std::setprecision(prec) << std::fixed;
      std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

      print_coverage(mode_cvp);
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

  }; // end of class minimal_scoped_cvp_coverage_b

  minimal_scoped_cvp_coverage_d  min_cvg_z_d1;
  minimal_scoped_cvp_coverage_d  min_cvg_z_d2;
};

class minimal_cvp_coverage : public covergroup
{
public :

  CG_CONS(minimal_cvp_coverage) { }
  minimal_cvp_coverage(std::string inst_name,
                const char *auto_inst_file_name, int auto_line,
                fc4sc::global* cntxt,
                int param) :
    fc4sc::covergroup("minimal_cvp_coverage",
                      __FILE__, __LINE__,
                      inst_name,
                      auto_inst_file_name, auto_line,
                      cntxt)
  {
    // handle param
  }
  minimal_cvp_coverage(std::string inst_name,
                const char *auto_inst_file_name, int auto_line,
                fc4sc::global* cntxt,
                double param) :
    minimal_cvp_coverage(inst_name,
                         auto_inst_file_name, auto_line,
                         cntxt)
  {
    // handle param
  }
  uint8_t  mode   {0};

  COVERPOINT( uint8_t, mode_cvp,  mode, true)
  {
    bin<uint8_t>("zero", 0),
    bin<uint8_t>("one",  1),
    bin<uint8_t>("two",  2)
  };

  void print_coverage() const
  {
    const int prec = 3;
    const int width = prec + 4; // 3 digits for "100" and the point
    std::cout << std::setprecision(prec) << std::fixed;
    std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

    print_coverage(mode_cvp);
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

class minimal_cvp_coverage_scoped : public covergroup
{
public :

  CG_SCOPED_CONS(minimal_cvp_coverage_scoped, scope_minimal, int a = 7) { }

  uint8_t  mode   {0};

  COVERPOINT( uint8_t, mode_cvp,  mode, true)
  {
    bin<uint8_t>("zero", 0),
    bin<uint8_t>("one",  1),
    bin<uint8_t>("two",  2)
  };

  void print_coverage() const
  {
    const int prec = 3;
    const int width = prec + 4; // 3 digits for "100" and the point
    std::cout << std::setprecision(prec) << std::fixed;
    std::cout << std::setw(width) << get_inst_coverage() << " % (complete covergroup)" << std::endl;

    print_coverage(mode_cvp);
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


void generate_minimal_cvp_scope()
{
  // 1 instance of scope_minimal in default context.
  // This scope contains one covergroup min_cvg_a1.

  std::string cov_file_name("minimal_cvp_scope");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  scope_minimal mod("mod", __FILE__, __LINE__);

  mod.min_cvg_a1.mode = 0;
  mod.min_cvg_a1.sample();

  mod.min_cvg_a1.mode = 2;
  mod.min_cvg_a1.sample();

  mod.min_cvg_a1.mode = 6;
  mod.min_cvg_a1.sample();

  mod.min_cvg_a1.stop();

  mod.min_cvg_a1.print_coverage();

  xml_printer::coverage_save(cov_file_name + ".xml");

  print_context_info(fc4sc::global::getter());
}

void generate_minimal_cvp_context_scope()
{
  // 2 instances of scope_minimal in context
  // This scope contains one covergroup min_cvg_a1.

  std::string cov_file_name("minimal_cvp_context_scope");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  fc4sc::global *cntxt = fc4sc::global::create_new_context();
  scope_minimal mod1("mod1", __FILE__, __LINE__, cntxt);
  scope_minimal mod2("mod2", __FILE__, __LINE__, cntxt);

  mod1.min_cvg_a1.mode = 0;
  mod1.min_cvg_a1.sample();

  mod1.min_cvg_a1.mode = 2;
  mod1.min_cvg_a1.sample();

  mod1.min_cvg_a1.mode = 6;
  mod1.min_cvg_a1.sample();

  mod1.min_cvg_a1.stop();

  mod1.min_cvg_a1.print_coverage();

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  print_context_info(cntxt);

  fc4sc::global::delete_context(cntxt);
}

void generate_minimal_cvp_context()
{
  // Just default scope in context.
  // 1 instance min_cvp of covergroup minimal_cvp_coverage in default scope.

  std::string cov_file_name("minimal_cvp_context");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  fc4sc::global *cntxt = fc4sc::global::create_new_context();

  minimal_cvp_coverage min_cvp("min_cvp",   __FILE__, __LINE__, cntxt);
  minimal_cvp_coverage min_cvp1("min_cvp1", __FILE__, __LINE__, cntxt, 5);
  minimal_cvp_coverage min_cvp2("min_cvp2", __FILE__, __LINE__, cntxt, 5.0);

  min_cvp.mode = 0;
  min_cvp.sample();

  min_cvp.mode = 2;
  min_cvp.sample();

  min_cvp.mode = 6;
  min_cvp.sample();

  min_cvp.stop();

  min_cvp.print_coverage();

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  print_context_info(cntxt);

  fc4sc::global::delete_context(cntxt);
}


void generate_minimal_cvp_context_scope_mixed()
{
  // 2 instances of scope_minimal and 1 instance of scope_minimal_z in context.
  // Each of those instances contain covergroups.
  // default_scope also in context.
  // default_scope contains 2 instances of covergroup minimal_cvp_coverage.

  std::string cov_file_name("minimal_cvp_context_scope_mixed");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  fc4sc::global *cntxt = fc4sc::global::create_new_context();
  scope_minimal   mod1("mod1", __FILE__, __LINE__, cntxt);
  scope_minimal   mod2("mod2", __FILE__, __LINE__, cntxt);
  scope_minimal_z mod_z1("mod_z1", __FILE__, __LINE__, cntxt);

  mod1.min_cvg_a1.mode = 0;
  mod1.min_cvg_a1.sample();

  mod1.min_cvg_a1.mode = 2;
  mod1.min_cvg_a1.sample();

  mod1.min_cvg_a1.mode = 6;
  mod1.min_cvg_a1.sample();

  mod1.min_cvg_a1.stop();

  minimal_cvp_coverage min_cvp("min_cvp", __FILE__, __LINE__, cntxt);
  minimal_cvp_coverage min_cvp2("min_cvp2", __FILE__, __LINE__, cntxt);

  min_cvp.mode = 0;
  min_cvp.sample();

  min_cvp.mode = 2;
  min_cvp.sample();

  min_cvp.mode = 6;
  min_cvp.sample();

  min_cvp.stop();

  min_cvp.print_coverage();

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  print_context_info(cntxt);

  fc4sc::global::delete_context(cntxt);
}

#define CG_SCOPED_INST_EXT(scp, name, args...) \
  name(scp,#name,__FILE__,__LINE__,##args)


void generate_minimal_cvp_context_scope_separated()
{
  std::string cov_file_name("minimal_cvp_context_scope_separated");

  std::cout << std::endl;
  std::cout << "----------------------------------------------" << std::endl;
  std::cout << "Generating " << cov_file_name << std::endl;

  fc4sc::global *cntxt = fc4sc::global::create_new_context();

  scope_minimal mod1("mod1", __FILE__, __LINE__, cntxt);

  minimal_cvp_coverage_scoped min_cvp(mod1, "min_cvp", __FILE__, __LINE__, 25);
  minimal_cvp_coverage_scoped CG_SCOPED_INST_EXT(mod1, min_cvp1);

  min_cvp.mode = 1;
  min_cvp.sample();

  min_cvp.stop();

  min_cvp.print_coverage();

  xml_printer::coverage_save(cov_file_name + ".xml", cntxt);

  print_context_info(cntxt);

  fc4sc::global::delete_context(cntxt);
}

void generate_minimal_cvp()
{
  generate_minimal_cvp_scope();
  generate_minimal_cvp_context_scope();
  generate_minimal_cvp_context();
  generate_minimal_cvp_context_scope_mixed();
  generate_minimal_cvp_context_scope_separated();
}
