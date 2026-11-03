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
contrib/tools/.gitignore \
"

for file in $add_files; do
  echo "Copying $file"
  cp $HTWD_CONTRIB/$file $FC4SC/$file
done

