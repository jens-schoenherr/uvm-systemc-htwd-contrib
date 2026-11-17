#!/bin/bash

# This script patches the FC4SC version of Accellera.
# It is assumed that contrib/fetch-tools.sh has been started before.

# directory that contains the FC4SC version of Accellera (https://github.com/accellera-official/fc4sc, commit 31b9339)
# run before this patch script:
#   cd contrib; ./fetch-tools.sh; cd ..
#   cd test; ./fetch-googletest.sh; cd ..
export FC4SC=~/git/fc4sc

# directory that contains a local copy of https://github.com/jens-schoenherr/uvm-systemc-htwd-contrib
export HTWD_CONTRIB=~/git/uvm-systemc-htwd-contrib/fc4sc

patch_files=" \
includes/fc4sc_base.hpp \
includes/fc4sc_binsof.hpp \
includes/fc4sc_coverpoint.hpp \
includes/fc4sc_master.hpp \
includes/fc4sc_scope.hpp \
includes/fc4sc_bin.hpp \
includes/fc4sc_covergroup.hpp \
includes/fc4sc_cross.hpp \
includes/fc4sc_intervals.hpp \
includes/fc4sc_options.hpp \
includes/xml_printer.hpp \
test/fc4sc/basic_control_test.cpp \
test/fc4sc/basic_coverpoint_definitions.cpp \
test/fc4sc/basic_cross_bins_filtering.cpp \
test/fc4sc/basic_cross_options_test.cpp \
test/fc4sc/basic_cvg_at_least_test.cpp \
test/fc4sc/basic_cvg_goal_test.cpp \
test/fc4sc/basic_cvg_weight_test.cpp \
test/fc4sc/basic_cvp_at_least_test.cpp \
test/fc4sc/basic_cvp_goal_test.cpp \
test/fc4sc/basic_cvp_sample_condition_test.cpp \
test/fc4sc/basic_cvp_sample_expression_test.cpp \
test/fc4sc/basic_cvp_weight_test.cpp \
test/fc4sc/bin_array_test.cpp \
test/fc4sc/bin_range_overlap.cpp \
test/fc4sc/cvg_get_inst_coverage_edge_cases.cpp \
test/fc4sc/cvp_general_expression.cpp \
test/fc4sc/cvp_get_bin_hit_count_test.cpp \
test/fc4sc/cvp_get_inst_coverage_edge_cases.cpp \
test/fc4sc/declaration_definition_covergroup.cpp \
test/fc4sc/demo_test.cpp \
test/fc4sc/dynamic_bin_test.cpp \
test/fc4sc/flexible_bin_array_test.cpp \
test/fc4sc/illegal_bin_test.cpp \
test/fc4sc/merge_test.cpp \
test/fc4sc/random_test_1.cpp \
test/fc4sc/scope_test.cpp \
contrib/tools/coverage_merge/merge.py \
contrib/tools/ucis_parser.py \
contrib/tools/gui/index.html \
contrib/tools/gui/fc4sc/js/bin.js \
contrib/tools/gui/fc4sc/js/bin_cross.js \
contrib/tools/gui/fc4sc/js/cg.js \
contrib/tools/gui/fc4sc/js/cross.js \
contrib/tools/gui/fc4sc/js/cvp.js \
"

for file in $patch_files; do
  patch -u $FC4SC/$file < $HTWD_CONTRIB/$file.patch 
done

add_files=" \
doc/fc4sc-users-guide.pdf \
includes/html_printer.hpp \
test/fc4sc/xml_compare.h \
test/fc4sc/xml_compare.cpp \
test/fc4sc/ref_files/basic_at_least0.xml \
test/fc4sc/ref_files/basic_at_least2.xml \
test/fc4sc/ref_files/basic_at_least.xml \
test/fc4sc/ref_files/basic_bin_hit_basic.xml \
test/fc4sc/ref_files/basic_binsof.xml \
test/fc4sc/ref_files/basic_control0.xml \
test/fc4sc/ref_files/basic_control11.xml \
test/fc4sc/ref_files/basic_control.xml \
test/fc4sc/ref_files/basic_edge_cross.xml \
test/fc4sc/ref_files/basic_goal0.xml \
test/fc4sc/ref_files/basic_goal1.xml \
test/fc4sc/ref_files/basic_goal.xml \
test/fc4sc/ref_files/basic_sample_condition.xml \
test/fc4sc/ref_files/basic_sample_expression.xml \
test/fc4sc/ref_files/basic_weight0.xml \
test/fc4sc/ref_files/basic_weight1.xml \
test/fc4sc/ref_files/basic_weight.xml \
test/fc4sc/ref_files/bin_array_edge_cases.xml \
test/fc4sc/ref_files/bin_overlap_signed_integer_2.xml \
test/fc4sc/ref_files/bin_overlap_signed_integer.xml \
test/fc4sc/ref_files/cvg_get_inst_coverage_all_zero_weights.xml \
test/fc4sc/ref_files/cvp_general_expression_global_var.xml \
test/fc4sc/ref_files/cvp_get_inst_coverage_all_zero_weights.xml \
test/fc4sc/ref_files/declaration_definition_covergroup_scoped.xml \
test/fc4sc/ref_files/declaration_definition_covergroup.xml \
test/fc4sc/ref_files/demo.xml \
test/fc4sc/ref_files/dynamic_bin_basic_test.xml \
test/fc4sc/ref_files/dynamic_bin_cross_test.xml \
test/fc4sc/ref_files/dynamic_covergroup_basic_test.xml \
test/fc4sc/ref_files/dynamic_covergroup_bin_array1.xml \
test/fc4sc/ref_files/dynamic_covergroup_bin_array2.xml \
test/fc4sc/ref_files/dynamic_covergroup_bin_array3.xml \
test/fc4sc/ref_files/dynamic_covergroup_cross_test.xml \
test/fc4sc/ref_files/dynamic_covergroup_ignore_bin.xml \
test/fc4sc/ref_files/dynamic_covergroup_illegal_bin.xml \
test/fc4sc/ref_files/dynamic_covergroup_scope_test_no_condition.xml \
test/fc4sc/ref_files/dynamic_covergroup_scope_test.xml \
test/fc4sc/ref_files/dynamic_scope_demo1.xml \
test/fc4sc/ref_files/dynamic_scope_test1.xml \
test/fc4sc/ref_files/flexible_bin_base.xml \
test/fc4sc/ref_files/merge_test1.xml \
test/fc4sc/ref_files/merge_test2.xml \
test/fc4sc/ref_files/merge_test3.xml \
test/fc4sc/ref_files/merge_test4.xml \
test/fc4sc/ref_files/random_1.xml \
test/fc4sc/ref_files/sample_slice.xml \
test/fc4sc/ref_files/scoped_demo.xml \
test/fc4sc/ref_files/scope_test1.xml \
test/fc4sc/ref_files/scope_test2.xml \
test/fc4sc/ref_files/toy_scope.xml \
examples/users_guide/default_scope/CMakeLists.txt \
examples/users_guide/default_scope/default_scope_example.cpp \
examples/users_guide/default_scope/uint8_cvp.cpp \
examples/users_guide/default_scope/uint8_cvp.h \
examples/users_guide/default_scope/wo_cvp.cpp \
examples/users_guide/default_scope/wo_cvp.h \
examples/users_guide/sampling/CMakeLists.txt \
examples/users_guide/sampling/bins.cpp \
examples/users_guide/sampling/bins.h \
examples/users_guide/sampling/sampling_example.cpp \
examples/users_guide/scoped/CMakeLists.txt \
examples/users_guide/scoped/context_info.cpp \
examples/users_guide/scoped/context_info.h \
examples/users_guide/scoped/dynamic_scope.cpp \
examples/users_guide/scoped/dynamic_scope.h \
examples/users_guide/scoped/enum_cvp.cpp \
examples/users_guide/scoped/enum_cvp.h \
examples/users_guide/scoped/hierarchical_scope.cpp \
examples/users_guide/scoped/hierarchical_scope.h \
examples/users_guide/scoped/minimal_cvp.cpp \
examples/users_guide/scoped/minimal_cvp.h \
examples/users_guide/scoped/scoped_example.cpp \
examples/users_guide/scoped/scoped.pdf \
examples/users_guide/scoped/uint8_cvp.cpp \
examples/users_guide/scoped/uint8_cvp.h \
examples/users_guide/scoped/wo_cvp.cpp \
examples/users_guide/scoped/wo_cvp.h \
examples/users_guide/.gitignore \
contrib/tools/.gitignore \
contrib/tests/gui/.gitignore \
contrib/tests/gui/test_gui.sh \
contrib/tests/gui/basics/readme.md \
contrib/tests/gui/basics/CMakeLists.txt \
contrib/tests/gui/basics/char_cvp.cpp \
contrib/tests/gui/basics/char_cvp.h \
contrib/tests/gui/basics/gui_test_basics.cpp \
contrib/tests/gui/basics/uint8_cvp.cpp \
contrib/tests/gui/basics/uint8_cvp.h \
contrib/tests/gui/basics/wo_cvp.cpp \
contrib/tests/gui/basics/wo_cvp.h \
contrib/tests/gui/basics/ref_files/test_gui_out.txt \
contrib/tests/gui/basics/ref_files/char_cvp_coverage_compact_3.html \
contrib/tests/gui/basics/ref_files/char_cvp_coverage_full_1.html \
contrib/tests/gui/basics/ref_files/char_cvp_coverage_partial_2.html \
contrib/tests/gui/basics/ref_files/char_cvp_coverage.xml.html \
contrib/tests/gui/basics/ref_files/uint8_cvp_coverage_compact_1.html \
contrib/tests/gui/basics/ref_files/uint8_cvp_coverage_full_2.html \
contrib/tests/gui/basics/ref_files/uint8_cvp_coverage_partial_3.html \
contrib/tests/gui/basics/ref_files/uint8_cvp_coverage.xml.html \
contrib/tests/gui/basics/ref_files/wo_cvp_coverage_compact_3.html \
contrib/tests/gui/basics/ref_files/wo_cvp_coverage_full_1.html \
contrib/tests/gui/basics/ref_files/wo_cvp_coverage_partial_2.html \
contrib/tests/gui/basics/ref_files/wo_cvp_coverage.xml.html \
contrib/tests/coverage_merge/.gitignore \
contrib/tests/coverage_merge/test_merge.sh \
contrib/tests/coverage_merge/basics/bool_cvp.cpp \
contrib/tests/coverage_merge/basics/bool_cvp.h \
contrib/tests/coverage_merge/basics/bool_uint8_cvp.cpp \
contrib/tests/coverage_merge/basics/bool_uint8_cvp.h \
contrib/tests/coverage_merge/basics/char_cvp.cpp \
contrib/tests/coverage_merge/basics/char_cvp.h \
contrib/tests/coverage_merge/basics/CMakeLists.txt \
contrib/tests/coverage_merge/basics/cross_without_bin_cvp.cpp \
contrib/tests/coverage_merge/basics/cross_without_bin_cvp.h \
contrib/tests/coverage_merge/basics/int_cvp.cpp \
contrib/tests/coverage_merge/basics/int_cvp.h \
contrib/tests/coverage_merge/basics/merge_test_basics.cpp \
contrib/tests/coverage_merge/basics/several_data_types_cvp.cpp \
contrib/tests/coverage_merge/basics/several_data_types_cvp.h \
contrib/tests/coverage_merge/basics/uint32_cvp.cpp \
contrib/tests/coverage_merge/basics/uint32_cvp.h \
contrib/tests/coverage_merge/basics/uint8_cvp.cpp \
contrib/tests/coverage_merge/basics/uint8_cvp.h \
contrib/tests/coverage_merge/basics/without_cross_test_cvp.cpp \
contrib/tests/coverage_merge/basics/without_cross_test_cvp.h \
contrib/tests/coverage_merge/basics/wo_cvp.cpp \
contrib/tests/coverage_merge/basics/wo_cvp.h \
contrib/tests/coverage_merge/basics/xml_file_compare.cpp \
contrib/tests/coverage_merge/basics/ref_files/bool_cov_scope_merged.xml \
contrib/tests/coverage_merge/basics/ref_files/bool_uint8_cov_scope_merged.xml \
contrib/tests/coverage_merge/basics/ref_files/char_cov_scope_merged.xml \
contrib/tests/coverage_merge/basics/ref_files/cross_without_bin_cov_scope_merged.xml \
contrib/tests/coverage_merge/basics/ref_files/int_cov_scope_merged.xml \
contrib/tests/coverage_merge/basics/ref_files/several_data_types_cov_scope_merged.xml \
contrib/tests/coverage_merge/basics/ref_files/uint32_cov_scope_merged.xml \
contrib/tests/coverage_merge/basics/ref_files/uint8_cov_scope_merged.xml \
contrib/tests/coverage_merge/basics/ref_files/without_cross_test_cov_scope_merged.xml \
"

for file in $add_files; do
  echo "Copying $file"
  dst_file_name=$FC4SC/$file
  dst_dir="${dst_file_name%/*}"
  mkdir -p $dst_dir
  cp $HTWD_CONTRIB/$file $dst_file_name
done

