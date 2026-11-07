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
contrib/tests/gui/basics/ref_files/test_html_out.txt \
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
"

for file in $add_files; do
  echo "Copying $file"
  dst_file_name=$FC4SC/$file
  dst_dir="${dst_file_name%/*}"
  mkdir -p $dst_dir
  cp $HTWD_CONTRIB/$file $dst_file_name
done

