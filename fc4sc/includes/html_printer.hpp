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
   Original Authors: Valentin Siegel,  HTW Dresden

               Date: 2026-09-28
******************************************************************************/
/******************************************************************************
   html_printer

   This class generates a HTML page directly.
   The data is collected using the visitor pattern from fc4sc::covVisitorBase.

   usage:
     html_printer::coverage_save("file_name.html");
     html_printer::coverage_save("file_name.html", cntxt);
******************************************************************************/
#ifndef HTML_PRINTER_HPP
#define HTML_PRINTER_HPP

#include "fc4sc_base.hpp"

#include <cmath>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


class html_printer : public fc4sc::covVisitorBase {

public:
  enum class table_format {
    full = 0,
    partial = 1,
    compact = 2
  };

private:

  /*! output stream to write to html file*/
  std::ostream& stream;

  /*! stream to print table rows of each coverpoint/cross */
  std::ostringstream cvg_body;

  /*! stream to print all bins of current coverpoint */
  std::ostringstream bin_rows;

  /*! weighted sum of the coverpoint/cross percentages of the current covergroup */
  double cvg_pct_sum = 0.0;
  /*! sum of the coverpoint/cross weights of the current covergroup */
  double cvg_weight_sum = 0.0;

  /*! counter for rowspan of a covergroup */
  int cvg_inst_lines = 0;

  /* counter to store passes/fails of coverpoint */
  int pass_bins = 0;
  int fail_bins = 0;

  int range_count = 0;

  /*! at_least option of the coverpoint currently visited; [9] reset to 1 afterwards */
  uint64_t cvp_at_least = 1;

  /*! store wether table is shown compact partial or full */
  const table_format format;

  /*! value to set precision of percentages, default value 2 */
  const int set_digets;


public:

  explicit html_printer(std::ostream& out_stream, table_format fmt = table_format::full, int dgts = 2) : stream(out_stream), format(fmt), set_digets(dgts) {}

  /*!
   * \brief function called to print html table with all the data
   * \param cntxt fc4sc-context, whoes data is printed
   * \param stream where to print data
   */
  void print_data_html(fc4sc::global* cntxt)
  {
    stream << "<!DOCTYPE html>\n"
           << "<html>\n"
           << "<head>\n"
           << "<meta charset=\"UTF-8\">\n"
           << "<title>fc4sc coverage table</title>\n"
           << "<style>" << style() << "</style>\n"
           << "</head>\n"
           << "<body>\n"
           << "<div id=\"file-content\" class=\"my_div\">\n\n";

    for (auto scope_inst_it : fc4sc::global::get_top_scopes(cntxt)) // visit each scope
    {
      scope_inst_it->accept_visitor(*this);
    }

    stream << "</div>\n"
           << "</body>\n"
           << "</html>\n";
  }

  /*!
   * \brief visit a scope instance
   */
  void visit(fc4sc::scp_base_data_model& base) override
  {
    bool has_enabled_cvg = false; // only visited if one active covergroup exists
    for (auto& type_it : base.cvgs)
    {
      for (auto cvg : type_it.second)
      {
        if (cvg->enable)
        {
          has_enabled_cvg = true;
          break;
        }
      }
    }

    if (has_enabled_cvg)
    {
      std::string type_name = escape_html_chars(base.type_data->type_name);
      std::string inst_name = escape_html_chars(base.name);

      stream << "<div style=\"text-align: center;\" id=\"" << type_name << "\">\n"
             << "<h1>" << type_name << " " << inst_name << "</h1>\n";

      for (auto& type_it : base.cvgs) // visit all covergroup of this scope covergroup type by covergroup type
      {
        for (auto cvg : type_it.second) // all covergroup instances of covergroup type "type_it"
        {
          if (cvg->enable)
          {
            cvg->accept_visitor(*this);
          }
        }
      }
      stream << "</div>\n";
    }

    for (auto scope_inst_it : base.child_scp_insts) // visit all subscopes
    {
      scope_inst_it.second->accept_visitor(*this);
    }
  }

  /*!
   * \brief visit a covergroup instance
   */
  void visit(fc4sc::cvg_base_data_model& base) override
  {
    cvg_body.str(""); //replace cvg_body with empty string
    cvg_body.clear(); //reset error flags
    cvg_pct_sum = 0.0;
    cvg_weight_sum = 0.0;
    cvg_inst_lines = 0;

    for (auto cvp : base.cvps) // visit all coverpoints and crosses
    {
      cvp->accept_visitor(*this);
    }

    double cvg_coverage;
    if (cvg_weight_sum == 0.0)
    {
      cvg_coverage = (base.option.weight == 0) ? 100.0 : 0.0;
    }
    else
    {
      cvg_coverage = round_digets(cvg_pct_sum / cvg_weight_sum);
      if (cvg_coverage >= base.option.goal)
      {
        cvg_coverage = 100.0; //set to 100% if goal was hit
      }
    }

    std::string style_class = get_pct_class(cvg_coverage);

    stream  << "<table id=\"cg_table\">\n\n"
            << "<tr>\n"
            << "<th class=\"pct " << style_class << "\" rowspan=\"" << (cvg_inst_lines + 1) << "\">" <<  fmt_pct(cvg_coverage) << "%" <<  "</th>\n"
            << "<th class=\"cg_inst\" colspan=\"" << (5 - colspan_sub(format)) << "\">" << escape_html_chars(base.type_data->type_name) << " " << escape_html_chars(base.name) << "</th>\n"
            << "</tr>\n"
            << cvg_body.str()
            << "</table>\n";
  }

  /*!
   * \brief visit a coverpoint
   */
  void visit(fc4sc::coverpoint_base_data_model& base) override
  {
    bin_rows.str("");
    bin_rows.clear();
    pass_bins = 0;
    fail_bins = 0;
    range_count = 0;

    cvp_at_least = base.option.at_least; //set minimun necessary hits to count bin for whole coverpoint

    for (auto bin : base.bins_data)
    {
      bin->accept_visitor(*this);
    }
    for (auto bin : base.illegal_bins_data)
    {
      bin->accept_visitor(*this);
    }
    for (auto bin : base.ignore_bins_data)
    {
      bin->accept_visitor(*this);   // ignore-bins are skipped in visit(bin)
    }

    cvp_at_least   = 1; //reset to 1 for next coverpoint visit
    int total_bins = pass_bins + fail_bins;

    double cvp_coverage = get_coverage(pass_bins, total_bins, base.option.weight, base.option.goal);
    int rowspan = (format == table_format::compact) ? 1 : (range_count + 1);

    cvg_body << "\n\n<!-- coverpoint: " << escape_html_chars(base.name) << " -->\n"
             << "<tr>"
             << "<td class=\"pct " << get_pct_class(cvp_coverage) << "\" rowspan=\"" << rowspan << "\">" << fmt_pct(cvp_coverage) << "%" <<   "</td>\n"
             << "<td class=\"cvp\" rowspan=\"1\" colspan=\"" << (4 - colspan_sub(format)) << "\">" << escape_html_chars(base.name) << " &quot;" << escape_html_chars(base.get_sample_expression_str()) << "&quot;" << "</td>"
             << "</tr>\n"
             << bin_rows.str();

    cvg_pct_sum   += cvp_coverage * static_cast<double>(base.option.weight);
    cvg_weight_sum += static_cast<double>(base.option.weight);

    cvg_inst_lines += rowspan; //add each coverpoints rowspan up to count covergroups rowspan
  }

  /*!
   * \brief visit a cross
   */
  void visit(fc4sc::cross_base_data_model& base) override
  {
    std::string expr; // create "cvp1 x cvp2 x ..." string
    uint64_t total_bins_cr = 1;
    uint64_t hit_bins_cr   = 0;

    for (size_t i = 0; i < base.cross_cvps.size(); ++i)
    {
      auto cvp = base.cross_cvps[i];
      if (i > 0)
      {
        expr += " x ";
      }

      expr += cvp->name;
      total_bins_cr *= cvp->size();
    }

    auto& cross_bins = base.get_cross_bins();

    for (auto& bin : cross_bins)
    {
      if (bin.second >= base.option.at_least) // get cross at_least
      {
        hit_bins_cr++;
      }
    }

    double cr_coverage = get_coverage(hit_bins_cr, total_bins_cr, base.option.weight, base.option.goal);
    int rowspan = (format == table_format::compact) ? 1 : (cross_bins.size() + 3);

    std::string name = base.name.empty() ? "cross coverage" : base.name;

    cvg_body << "\n\n<!-- coverpoint cross: " << escape_html_chars(name) << " -->\n"
             << "<tr>\n"
             << "<td class=\"pct " << get_pct_class(cr_coverage) << "\" rowspan=\"" << rowspan << "\">" << fmt_pct(cr_coverage) << "%" << "</td>\n";

    if(format != table_format::compact)
    {
      cvg_body << "</tr>\n\n"
               << "<tr>\n";
    }

    cvg_body << "<td class=\"cvp\" colspan=\"" << (4 - colspan_sub(format)) << "\">" << escape_html_chars(name) << "</td>\n"
             << "</tr>\n";

    if(format != table_format::compact)
    {
      cvg_body << "<tr>\n"
               << "<td class=\"cvp\" colspan=\"" << (2 - colspan_sub(format)) << "\">" << escape_html_chars(expr) << "</td>\n"
               << "<td class=\"cvp\" colspan=\"2\">" << hit_bins_cr << "/" << total_bins_cr << " bins hit</td>\n"
               << "</tr>\n";

      for (auto& bin : cross_bins)
      {
        bool hit = bin.second >= base.option.at_least;

        cvg_body << "<tr class=\"bin_" << (hit ? "pass" : "fail") << "\">\n"
                 << "<td colspan=\"" << (2 - colspan_sub(format)) << "\">{";

        for (size_t i = 0; i < bin.first.size(); ++i)
        {
          if (i > 0)
          {
            cvg_body << ",";
          }

          cvg_body << bin.first[i];
        }

        cvg_body << "}</td>\n"
                 << "<td>" << bin.second << "</td>\n"
                 << "<td>" << (hit ? "&#10004;" : "&#10008;") << "</td>\n"
                 << "</tr>\n";
      }
    }

    cvg_pct_sum += cr_coverage * static_cast<double>(base.option.weight);
    cvg_weight_sum += static_cast<double>(base.option.weight);

    cvg_inst_lines += rowspan; //add each coverpoints rowspan up to count covergroups rowspan
  }

  /*!
   * \brief visit a bin
   */
  void visit(fc4sc::bin_base_data_model& base) override
  {
    fc4sc::bin_t type = base.get_bin_type();

    if (type == fc4sc::bin_t::ignore_)  // bins of type "ingore" are ignored
    {
      return;
    }

    bool illegal = (type == fc4sc::bin_t::illegal_);

    auto intervals = base.get_intervals_to_int();
    auto& hits = base.get_interval_hits();

    uint64_t total_hits = 0;
    for (auto h : hits)
    {
      total_hits += h;
    }

    std::vector<std::string> ranges; //store names of coverpointBin

    if(format == table_format::full)
    {
      for (auto& interval : intervals)
      {
        if (interval.first == interval.second)
        {
          ranges.push_back(std::to_string(interval.first));
        }
        else
        {
          ranges.push_back("[" + std::to_string(interval.first) + ":" + std::to_string(interval.second) + "]");
        }
      }
    }

    if (ranges.empty())
    {
      ranges.push_back("-");
    }

    range_count += static_cast<int>(ranges.size());

    if (!illegal)
    {
      if (total_hits >= cvp_at_least)  //cvp_at_least set by visit(coverpoint) before bins are visited
      {
        pass_bins++;
      }
      else
      {
        fail_bins++;
      }
    }

    if(format != table_format::compact)
    {
      std::string style_class;
      std::string tick;
      if (illegal) //style if illegal
      {
        style_class = (total_hits < cvp_at_least) ? "bin_illegal" : "bin_fail";
        tick = (total_hits < cvp_at_least) ? "&#10004;" : "&#10008;";
      }
      else //style if default
      {
        style_class = (total_hits  >= cvp_at_least) ? "bin_pass" : "bin_fail";
        tick = (total_hits >= cvp_at_least) ? "&#10004;" : "&#10008;";
      }

      size_t rowspan = (format == table_format::full) ? ranges.size() : 1;

      bin_rows << "\n<tr class=\"" << style_class << "\">\n"
               << "<td class=\"bin_header\" rowspan=\"" << rowspan << "\">"
               << escape_html_chars(base.get_name())
               << "</td>\n";

      if(format == table_format::full)
      {
        bin_rows << "<td>" << ranges[0] << "</td>\n";
      }

      bin_rows << "<td rowspan=\"" << rowspan << "\">" << total_hits << "</td>\n"
               << "<td rowspan=\"" << rowspan << "\">" << tick << "</td>\n"
               << "</tr>\n";

      if(format == table_format::full)
      {
        for (size_t i = 1; i < ranges.size(); ++i)
        {
        bin_rows << "<tr class=\"" << style_class << "\">\n"
                 << "<td>" << ranges[i] << "</td>\n"
                 << "</tr>\n";
        }
      }
    }
  }

  /*!
   * \brief prints data to given file name
   * \param file_name where to print, returns if empty
   */
  static void coverage_save(const std::string &file_name = "", fc4sc::global* cntxt = fc4sc::global::getter(), const table_format fmt = table_format::full, const int dgts = 2)
  {
    if (file_name.empty())
    {
      std::cerr << "FC4SC " << __FUNCTION__ << ": Error! Empty string was passed to function as file name\n";
      std::cerr << "COVERAGE HTML REPORT HAS NOT BEEN SAVED!" << std::endl;
      return;
    }

    std::ofstream file(file_name);

    if (!file)
    {
      std::cerr << "FC4SC " << __FUNCTION__ << ": Error! Could not open file [" << file_name << "] for writing!" << std::endl;
      std::cerr << "COVERAGE HTML REPORT HAS NOT BEEN SAVED!" << std::endl;
      return;
    }
    coverage_save(file, cntxt, fmt, dgts);
  }

  /*!
   * \brief prints data to the given stream
   * \param stream object where to print
   */
  static void coverage_save(std::ostream& stream, fc4sc::global* cntxt = fc4sc::global::getter(), const table_format format = table_format::full, const int dgts = 2)
  {
    html_printer printer(stream, format, dgts);
    printer.print_data_html(cntxt);
  }

private:
   /*! formates percentage to two digets*/
  std::string fmt_pct(double value)
  {
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(set_digets) << value;
    return ss.str();
  }

  /*! round to two digets */
  double round_digets(double value)
  {
    double power = std::pow(10.0, set_digets);
    return std::round(value * power) / power;
  }

  int colspan_sub(table_format format) const
  {
    switch (format)
    {
      case table_format::compact: return 3;
      case table_format::partial: return 1;
      case table_format::full: return 0;
    }
    return 0; //default table format full
  }

  /*!
   * \brief coverage of one object: hit bins / total bins, in percent
   */
  double get_coverage(uint64_t hit, uint64_t total, uint64_t weight, uint64_t goal)
  {
    if (total == 0)
    {
      return (weight == 0) ? 100.0 : 0.0;
    }
    double coverage = round_digets(100.0 * hit / total);
    return (coverage >= goal) ? 100.0 : coverage; //set to 100% if goal was hit
  }

  /*! css class for a percentage cell */
  static std::string get_pct_class(double coverage)
  {
    if (coverage >= 100.0)
    {
      return "pct_full";
    }
    if (coverage <= 0.0)
    {
      return "pct_empty";
    }
    return "pct_partial";
  }

   /*!
   * \brief excludes all special html characters
   */
  static std::string escape_html_chars(const std::string &in)
  {
    std::string out;
    out.reserve(in.size());
    for (char c : in)
    {
      switch (c)
      {
        case '<':  out += "&lt;";   break;
        case '>':  out += "&gt;";   break;
        case '&':  out += "&amp;";  break;
        case '\"': out += "&quot;"; break;
        case '\'': out += "&#39;";  break;
        default:   out += c;
      }
    }
    return out;
  }

  /*! function which returns string containing css code for <style> element */
  static const char* style()
  {
    /* The CSS uses elements originaly provided by AMIQ Consulting in the file fc4sc.css.
     * The content of this file is adapted, extended and copied to each HTML file created with this class.
     * This allows viewing the generated HTML file without any extra css-file.
     */
    return R"CSS(
body {
  font-family: monospace;
  font-size: 1rem;
}

table#cg_table {
  border-spacing: 0;
  border-collapse: separate;
  border-top: 1px solid black;
  border-left: 1px solid black;
  margin-bottom: 32px;
}

table#cg_table td, table#cg_table th {
  border: 0;
  border-right: 1px solid black;
  border-bottom: 1px solid black;
}

/******************************************************************************

  Copyright 2003-2018 AMIQ Consulting s.r.l.

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
  Original Authors: Teodor Vasilache and Dragos Dospinescu,
                    AMIQ Consulting s.r.l. (contributors@amiq.com)

              Date: 2018-Feb-20

  Modified by: Valentin Siegel, HTW Dresden
               (Adaptions for usage without extra css file)

               Date: 2026-09-28
******************************************************************************/
.cvp {
  font-size: 120%;
  font-weight: bold;
}

.cg_inst {
  font-size: 170%;
  font-weight: bold;
}

.bin_fail {
  color : red;
  font-weight: bold;
  left: 30%;
}

.bin_pass {
  color : green;
  font-weight: bold;
  left: 30%;
}

.bin_illegal {
  color : #77d47a;
  font-weight: bold;
  left: 30%;
}

table#cg_table {
  width: 100%;
  overflow-x: inherit
}

td,tr,th {
  text-align: center;
  border: 1px solid black;
}

.my_div {
  width: 100%;
  max-width: 1000px;
  box-sizing: border-box;
  position:absolute;
  left:50%;
  padding:15px;
  -ms-transform: translateX(-50%);
  -webkit-transform: translate(-50%,0%);
  transform: translate(-50%,0%);
}

.pct {
  font-size: 200%;
}

.pct_excluded {
  color: gray;
}

.pct_empty {
  color: red;
}

.pct_partial {
  color: #ebc944;
}

.pct_full {
  color: green;
}
)CSS";

  }

};

#endif
