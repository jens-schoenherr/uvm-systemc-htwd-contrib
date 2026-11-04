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
#include "context_info.h"

void print_scope(              fc4sc::scp_base_data_model *scp,  std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt);
void print_scope_metadata(     fc4sc::scp_metadata *td,          std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt);
void print_covergroup(         fc4sc::cvg_base_data_model *cg,   std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt);
void print_covergroup_metadata(fc4sc::cvg_metadata *cm,          std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt);
void print_coverpoint(         fc4sc::cvp_base_data_model *cp,   std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt);
void print_actual_coverpoint(  fc4sc::coverpoint_data_model *cp, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt);
void print_bin(                fc4sc::bin_base_data_model *b,    std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt);
void print_cross(              fc4sc::cross_data_model *cp,      std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt);

std::string get_file_name(unsigned int file_id, uint32_t line, fc4sc::global *cntxt);
std::string get_valid(bool valid);
std::string get_enabled(bool enabled);

void print_context_info(fc4sc::global *cntxt)
{
  double cov = fc4sc::global::get_coverage(cntxt);
  std::cout << std::endl << "Context coverage: " << cov << " %" << std::endl;

  std::cout << "scope types" << std::endl;
  std::unordered_map<std::string,fc4sc::scp_metadata*> scp_types = fc4sc::global::get_scopes_data(cntxt);
  std::cout << "-------------------" << std::endl;
  for (auto scp_type : scp_types)
  {
    print_scope_metadata(scp_type.second, "  ", true, true, cntxt);
  }

  std::cout << "top scopes" << std::endl;
  std::vector<fc4sc::scp_base_data_model*> top_sps = fc4sc::global::get_top_scopes(cntxt);
  for (fc4sc::scp_base_data_model* scp : top_sps)
  {
    std::cout << "-------------------" << std::endl;
    print_scope(scp, "  ", true, true, cntxt);
  }
}

void print_scope(fc4sc::scp_base_data_model *scp, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt)
{
  std::string prefix_sub = prefix + "  ";

  std::cout << prefix << "scope instance: " << scp->name << " (id=" << scp->instance_id << ", scope type: " << scp->type_data->type_name << ")" << std::endl;

  if (!structure_only)
  {
    std::cout << prefix_sub << get_file_name(scp->inst_file_id, scp->inst_line, cntxt) << std::endl;
    std::cout << prefix_sub << get_valid(*(scp->valid)) << std::endl;
    if (scp->cntxt != cntxt)
      std::cout << prefix_sub << "this context != context" << std::endl;

    std::cout << prefix_sub << "anonymous_count: " << scp->anonymous_count << std::endl;
  }

  if (scp->parent_scp == nullptr)
  {
    std::cout << prefix_sub << "no parent scope (top)" << std::endl;
  }
  else
  {
    std::cout << prefix_sub << "parent scope: " << scp->parent_scp->name << std::endl;
  }

  std::unordered_map<std::string, std::vector<fc4sc::scp_base_data_model*> > child_scopes = scp->child_scps;
  std::cout << prefix_sub << "child scope types (" << child_scopes.size() << ")" << std::endl;
  for (std::pair<std::string, std::vector<fc4sc::scp_base_data_model*> > cs: child_scopes)
  {
    std::cout << prefix_sub << "  " << cs.first << std::endl;
  }

  std::unordered_map<std::string, fc4sc::scp_base_data_model*> child_scope_insts = scp->child_scp_insts;
  std::cout << prefix_sub << "child scope instances (" << child_scope_insts.size() << ")" << std::endl;
  for (std::pair<std::string, fc4sc::scp_base_data_model*> csi: child_scope_insts)
  {
    std::cout << prefix_sub << "  " << csi.first << std::endl;
  }

  std::unordered_map<std::string, std::vector<fc4sc::cvg_base_data_model*> > cov_groups = scp->cvgs;
  std::cout << prefix_sub << "covergroup types (" << cov_groups.size() << ")" << std::endl;
  for (std::pair<std::string, std::vector<fc4sc::cvg_base_data_model*> > cg: cov_groups)
  {
    std::cout << prefix_sub << "  " << cg.first << std::endl;
    // print the names of the instances only if !forward_only
  }

  std::unordered_map<std::string, fc4sc::cvg_base_data_model*> cov_group_insts = scp->cvg_insts;
  std::cout << prefix_sub << "covergroup instances (" << cov_group_insts.size() << ")" << std::endl;
  for (std::pair<const std::string, fc4sc::cvg_base_data_model*> cgi: cov_group_insts)
  {
    print_covergroup(cgi.second, prefix_sub + "  ", forward_only, structure_only, cntxt);
  }

  std::cout << prefix_sub << "child scope instances (" << child_scope_insts.size() << ")" << std::endl;
  for (std::pair<std::string, fc4sc::scp_base_data_model*> csi: child_scope_insts)
  {
    print_scope(csi.second, prefix_sub + "  ", forward_only, structure_only, cntxt);
  }

}


void print_scope_metadata(fc4sc::scp_metadata *td, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt)
{
  std::string prefix_sub = prefix + "  ";

  std::cout << prefix << "scope type: " << td->type_name << std::endl;

  if (!structure_only)
  {
    std::cout << prefix_sub << get_file_name(td->file_id, td->line, cntxt)  << std::endl;
  }

  std::unordered_map<std::string, fc4sc::cvg_metadata*> cg_types = td->cvg_type_table;
  std::cout << prefix_sub << "covergroup types (" << cg_types.size() << ")" << std::endl;
  for (std::pair<std::string, fc4sc::cvg_metadata*> cgt: cg_types)
  {
    std::cout << prefix_sub << "  cg type: " << cgt.first << std::endl;
    if (!forward_only)
    {
      print_covergroup_metadata(cgt.second, prefix_sub + "  ", forward_only, structure_only, cntxt);
    }
  }

  if (!forward_only)
  {
    std::vector<fc4sc::scp_base_data_model*> scope_instances = td->scp_insts;
    std::cout << prefix_sub << "scope instances (" << scope_instances.size() << ")" << std::endl;
    for (fc4sc::scp_base_data_model* sci: scope_instances)
    {
      // sci is a back pointer to the scope
      std::cout <<  prefix_sub << "  " << sci->name << std::endl;
    }
  }
}

void print_covergroup(fc4sc::cvg_base_data_model *cg, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt)
{
  std::string prefix_sub = prefix + "  ";

  std::cout << prefix << "cg instance: " << cg->name << " (type: " << cg->type_data->type_name << ")" << std::endl;
  if (!forward_only)
  {
    std::cout << prefix_sub << "scope instance: " << cg->parent_scp->name << std::endl;
    std::cout << prefix_sub << "cg type name: " << cg->type_data->type_name << std::endl;
  }

  if (!structure_only)
  {
    std::cout << prefix_sub << get_enabled(cg->enable) << std::endl;
    std::cout << prefix_sub << get_file_name(cg->inst_file_id, cg->inst_line, cntxt) << std::endl;
    std::cout << prefix_sub << "  at least:                " << cg->option.at_least << std::endl;
    std::cout << prefix_sub << "  auto_bin_max:            " << cg->option.auto_bin_max << std::endl;
    std::cout << prefix_sub << "  comment:                 " << cg->option.comment << std::endl;
    std::cout << prefix_sub << "  cross_num_print_missing: " << cg->option.cross_num_print_missing << std::endl;
    std::cout << prefix_sub << "  detect_overlap:          " << cg->option.detect_overlap << std::endl;
    std::cout << prefix_sub << "  goal:                    " << cg->option.goal << std::endl;
    std::cout << prefix_sub << "  per_instance:            " << cg->option.per_instance << std::endl;
    std::cout << prefix_sub << "  weight:                  " << cg->option.weight << std::endl;
    std::cout << prefix_sub << "  least:                   " << cg->option.at_least << std::endl;
  }

  std::cout << prefix_sub << "coverpoints (" << cg->cvps.size() << ")" << std::endl;
  for (fc4sc::cvp_base_data_model* cvp: cg->cvps)
  {
    print_coverpoint(cvp, prefix_sub + "  ", forward_only, structure_only, cntxt);
  }
}

void print_covergroup_metadata(fc4sc::cvg_metadata *cm, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt)
{
  std::string prefix_sub = prefix + "  ";

  std::cout << prefix << "cg type: " << cm->type_name << std::endl;
  std::cout << prefix_sub << "scope type: " << cm->scp_type_name << std::endl;

  std::cout << prefix_sub << "covergroup instances (" << cm->cvg_insts.size() << ")" << std::endl;
  for (fc4sc::cvg_base_data_model* cgi: cm->cvg_insts)
  {
    // pointer to all instances
    std::cout << prefix_sub << "  " << cgi->name << std::endl;
  }

  if (!structure_only)
  {
    std::cout << prefix_sub << get_file_name(cm->file_id, cm->line, cntxt) << std::endl;
    std::cout << prefix_sub << "comment:         " << cm->type_option.comment << std::endl;
    std::cout << prefix_sub << "goal:            " << cm->type_option.goal << std::endl;
    std::cout << prefix_sub << "merge_instances: " << cm->type_option.merge_instances << std::endl;
    std::cout << prefix_sub << "weight:          " << cm->type_option.weight << std::endl;
  }
}

void print_coverpoint(fc4sc::cvp_base_data_model *cp, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt)
{
  std::string prefix_sub = prefix + "  ";

  fc4sc::coverpoint_data_model *cpt = dynamic_cast<fc4sc::coverpoint_data_model *>(cp);
  fc4sc::cross_data_model      *cpc = dynamic_cast<fc4sc::cross_data_model      *>(cp);
  if (cpt != nullptr)
  {
    print_actual_coverpoint(cpt, prefix_sub, forward_only, structure_only, cntxt);
  }
  else if (cpc != nullptr)
  {
    print_cross(cpc, prefix_sub, forward_only, structure_only, cntxt);
  }
  else
  {
    std::cout << prefix_sub << "neither a coverpoint nor a cross" << std::endl;
  }
  /* This style is not recomented because the check for nullptr after dynamic_cast<> should be implemented in addition.
  if (typeid(*cp) == typeid(fc4sc::coverpoint_data_model))
  {
    std::cout << prefix_sub << "this is a coverpoint" << std::endl;
  }
  else if (typeid(*cp) == typeid(fc4sc::cross_data_model))
  {
    std::cout << prefix_sub << "this is a cross" << std::endl;
  }
  else
  {
    std::cout << prefix_sub << "neither a coverpoint nor a cross" << std::endl;
  } */

  if (!structure_only)
  {
    std::cout << prefix_sub << "misses:  " << cp->misses << std::endl;
    std::cout << prefix_sub << get_valid(*(cp->valid)) << std::endl;
    std::cout << prefix_sub << "size:  " << cp->size() << std::endl;
  }
}

void print_actual_coverpoint(fc4sc::coverpoint_data_model *cp, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt)
{
  std::string prefix_sub = prefix + "  ";

  std::cout << prefix << "coverpoint " << cp->name << std::endl;

  if (!structure_only)
  {
    std::cout << prefix_sub << "sample expression: " << cp->sample_expression_str << std::endl;
    std::cout << prefix_sub << "sample condition:  " << cp->sample_condition_str << std::endl;
    std::cout << prefix_sub << "at_least:          " << cp->option.at_least << std::endl;
    std::cout << prefix_sub << "auto_bin_max:      " << cp->option.auto_bin_max << std::endl;
    std::cout << prefix_sub << "comment:           " << cp->option.comment << std::endl;
    std::cout << prefix_sub << "detect_overlap:    " << cp->option.detect_overlap << std::endl;
    std::cout << prefix_sub << "goal:              " << cp->option.goal << std::endl;
    std::cout << prefix_sub << "weight:            " << cp->option.weight << std::endl;
  }

  std::cout << prefix_sub << "regular bins (" << cp->bins_data.size() << ")" << std::endl;
  for (fc4sc::bin_base_data_model *b: cp->bins_data)
  {
    print_bin(b, prefix_sub + "  ", forward_only, structure_only, cntxt);
  }
  std::cout << prefix_sub << "ignore bins  (" << cp->ignore_bins_data.size() << ")" << std::endl;
  for (fc4sc::bin_base_data_model *b: cp->ignore_bins_data)
  {
    print_bin(b, prefix_sub + "  ", forward_only, structure_only, cntxt);
  }
  std::cout << prefix_sub << "illegal bins (" << cp->illegal_bins_data.size() << ")" << std::endl;
  for (fc4sc::bin_base_data_model *b: cp->illegal_bins_data)
  {
    print_bin(b, prefix_sub + "  ", forward_only, structure_only, cntxt);
  }
}

void print_bin(fc4sc::bin_base_data_model *b, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt)
{
  std::string prefix_sub = prefix + "  ";

  std::cout << prefix << "bin " << b->get_name() << std::endl;
  if (!structure_only)
  {
    std::vector<uint64_t>                hits      = b->get_interval_hits();
    std::vector<fc4sc::interval_t<int> > intervals = b->get_intervals_to_int();
    if (hits.size() != intervals.size())
    {
      std::cout << prefix_sub << "intervals and hints inconsistent" << std::endl;
    }
    else
    {
      for (size_t i = 0; i<intervals.size(); i++)
      {
        std::cout << prefix_sub << intervals[i].first << ".." << intervals[i].second << " : " << hits[i] <<  std::endl;
      }
    }
  }
}

void print_cross(fc4sc::cross_data_model *cp, std::string prefix, bool forward_only, bool structure_only, fc4sc::global *cntxt)
{
  std::string prefix_sub = prefix + "  ";

  std::cout << prefix << "cross " << cp->name << std::endl;
  if (!structure_only)
  {
    std::cout << prefix_sub << "at_least:          " << cp->option.at_least << std::endl;
    std::cout << prefix_sub << "cross_num_print_missing: " << cp->option.cross_num_print_missing << std::endl;
    std::cout << prefix_sub << "comment:           " << cp->option.comment << std::endl;
    std::cout << prefix_sub << "goal:              " << cp->option.goal << std::endl;
    std::cout << prefix_sub << "weight:            " << cp->option.weight << std::endl;
  }
  std::vector<fc4sc::cvp_base_data_model *> cvps = cp->cross_cvps;
  for (fc4sc::cvp_base_data_model *cpt : cvps)
  {
    std::cout << prefix_sub << "  " << cpt->name << std::endl;
  }
  if (!structure_only)
  {
    std::map<std::vector<size_t>, uint64_t> bins = cp->bins;
    for (std::pair<std::vector<size_t>, uint64_t > b: bins)
    {
      std::cout << prefix_sub << "(";
      std::vector<size_t> vals = b.first;
      for (size_t val: vals)
      {
        std::cout << " " << val;
      }
      std::cout << ") : " << b.second; // hit count
    }
  }
}

std::string get_file_name(unsigned int file_id, uint32_t line, fc4sc::global *cntxt)
{
  std::string result;
  std::unordered_map<unsigned int,std::string>::const_iterator file_it = fc4sc::global::get_file_id_to_name_table(cntxt).find(file_id);
  if (file_it == fc4sc::global::get_file_id_to_name_table(cntxt).cend())
  {
    result = "file-id " + std::to_string(file_id) + ":" + std::to_string(line);
  }
  else
  {
    result = "file: " + std::to_string(file_id) + " -> " + file_it->second +  ":" + std::to_string(line);
  }
  return result;
}

std::string get_valid(bool valid)
{
  if (valid)
    return "valid";
  else
    return "invalid";
}

std::string get_enabled(bool enabled)
{
  if (enabled)
    return "enabled";
  else
    return "disabled";
}
