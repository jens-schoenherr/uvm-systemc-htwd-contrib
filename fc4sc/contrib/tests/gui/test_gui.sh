#!/bin/bash

cd basics

rm -rf test_gui.log
touch  test_gui.log

echo "================================="  | tee -a test_gui.log
echo "Start of gui testing"               | tee -a test_gui.log
echo                                      | tee -a test_gui.log
echo "create example html files"          | tee -a test_gui.log
echo                                      | tee -a test_gui.log
build/gui_test_basics                2>&1 >        test_gui_out.txt

file=test_gui_out.txt
echo "=================================" | tee -a test_gui.log
echo "Test output for $file"             | tee -a test_gui.log
echo                                     | tee -a test_gui.log
echo "Differences:"                      | tee -a test_gui.log
diff "${file}" "ref_files/${file}"  2>&1 | tee -a test_gui.log
echo                                     | tee -a test_gui.log

files="\
char_cvp_coverage_compact_3  \
char_cvp_coverage_full_1     \
char_cvp_coverage_partial_2  \
uint8_cvp_coverage_compact_1 \
uint8_cvp_coverage_full_2    \
uint8_cvp_coverage_partial_3 \
wo_cvp_coverage_compact_3    \
wo_cvp_coverage_full_1       \
wo_cvp_coverage_partial_2    \
"

for file in $files; do
  echo "================================="           | tee -a test_gui.log
  echo "Test html for $file"                         | tee -a test_gui.log
  echo                                               | tee -a test_gui.log
  echo "Differences:"                                | tee -a test_gui.log
  diff "${file}.html" "ref_files/${file}.html"  2>&1 | tee -a test_gui.log
  echo                                               | tee -a test_gui.log
done

files="\
char_cvp_coverage.xml.html  \
uint8_cvp_coverage.xml.html \
wo_cvp_coverage.xml.html    \
"

for file in $files; do
  echo "================================="           | tee -a test_gui.log
  echo "Test xml/html for $file"                     | tee -a test_gui.log
  echo                                               | tee -a test_gui.log
  if [ -r  "$file" -a -f "$file" ]; then
    echo "Differences:"                                | tee -a test_gui.log
    diff "${file}" "ref_files/${file}"            2>&1 | tee -a test_gui.log
    echo                                               | tee -a test_gui.log
  else
    echo "File $file not avaliable for comparison."           | tee -a test_gui.log
    echo "It should be created manually using a HTML browser" | tee -a test_gui.log
    echo                                                      | tee -a test_gui.log
  fi
done

echo
echo "End of gui testing"                 | tee -a test_gui.log
echo "================================="  | tee -a test_gui.log

