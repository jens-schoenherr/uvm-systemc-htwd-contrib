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

success=1
failed_tests=
skipped_tests=

file=test_gui_out.txt
echo "=================================" | tee -a test_gui.log
echo "Test output for $file"             | tee -a test_gui.log
echo                                     | tee -a test_gui.log
echo "Differences:"                      | tee -a test_gui.log
diff "${file}" "ref_files/${file}"  2>&1 | tee -a test_gui.log
compare_result=${PIPESTATUS[0]}
if [ "$compare_result" -ne "0" ]; then
  success=0
  failed_tests="$failed_tests test_gui_out.txt"
fi
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
  compare_result=${PIPESTATUS[0]}
  if [ "$compare_result" -ne "0" ]; then
    success=0
    failed_tests="$failed_tests xml:$file"
  fi
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
    if [ "$compare_result" -ne "0" ]; then
      success=0
      failed_tests="$failed_tests html:$file"
    fi
    echo                                               | tee -a test_gui.log
  else
    echo "File $file not avaliable for comparison."           | tee -a test_gui.log
    echo "It should be created manually using a HTML browser" | tee -a test_gui.log
    echo                                                      | tee -a test_gui.log
    skipped_tests="$skipped_tests html:$file"
  fi
done

echo

# summary
echo "================================="                                                                              | tee -a test_gui.log
if [ "$success" -eq "1" ]; then
  echo "All tests passed."                                                                                            | tee -a test_gui.log
else
  echo "The following tests failed:"                                                                                  | tee -a test_gui.log
  for failed_test in $failed_tests; do
    echo "  $failed_test"                                                                                             | tee -a test_gui.log
  done
fi
echo "Skipped tests:"                                                                                                 | tee -a test_gui.log
for skipped_test in $skipped_tests; do
  echo "  $skipped_test"                                                                                              | tee -a test_gui.log
done

echo "================================="                                                                              | tee -a test_gui.log
echo "End of gui testing"                 | tee -a test_gui.log
echo "================================="  | tee -a test_gui.log

