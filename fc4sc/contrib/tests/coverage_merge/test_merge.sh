#!/bin/bash


# Path to the comparison tool:
COMPARE_BIN="./build/xml_file_compare"

cd basics

rm -rf test_merge.log
touch  test_merge.log

echo "================================="  | tee -a test_merge.log
echo "Start of merge testing"             | tee -a test_merge.log
echo "================================="  | tee -a test_merge.log
echo                                      | tee -a test_merge.log
echo "create example all xml files"       | tee -a test_merge.log
echo                                      | tee -a test_merge.log
rm -f *.xml
./build/merge_test_basics            2>&1 | tee -a test_merge.log
echo                                      | tee -a test_merge.log

files="\
bool_cov_scope \
bool_uint8_cov_scope \
char_cov_scope \
cross_without_bin_cov_scope \
int_cov_scope \
several_data_types_cov_scope \
uint8_cov_scope \
uint32_cov_scope \
without_cross_test_cov_scope \
"

success=1
failed_tests=

# command to test the merging of files a and b:
for file in $files; do
  echo "================================="                                                                            | tee -a test_merge.log
  echo "1. Merge test for $file"                                                                                      | tee -a test_merge.log
  echo                                                                                                                | tee -a test_merge.log
  python3 ../../../tools/coverage_merge/merge.py --merge_to_db=${file}_merged.xml ${file}_a.xml ${file}_b.xml    2>&1 | tee -a test_merge.log
  echo                                                                                                                | tee -a test_merge.log
  echo "Differences"                                                                                                  | tee -a test_merge.log
  # Calling the comparison tool: Expected XML first, then the generated XML
  $COMPARE_BIN "ref_files/${file}_merged.xml" "${file}_merged.xml"                                               2>&1 | tee -a test_merge.log
  compare_result=${PIPESTATUS[0]}
  if [ "$compare_result" -ne "0" ]; then
    success=0
    failed_tests="$failed_tests 1:$file"
  fi
  echo                                                                                                                | tee -a test_merge.log
done


# new command:
for file in $files; do
  echo "================================="                                                                            | tee -a test_merge.log
  echo "2. Merge test for $file"                                                                                      | tee -a test_merge.log
  echo                                                                                                                | tee -a test_merge.log
  mkdir -p "${file}_folder" # new folder
  cp "${file}_a.xml" "${file}_folder/" # copy file a into new folder
  cp "${file}_b.xml" "${file}_folder/" # copy file b into new folder
  python3 ../../../tools/coverage_merge/merge.py ${file}_folder ../${file}_merged_2.xml                          2>&1 | tee -a test_merge.log
  echo                                                                                                                | tee -a test_merge.log
  echo "Differences"                                                                                                  | tee -a test_merge.log
  # Calling the comparison tool: Expected XML first, then the generated XML
  $COMPARE_BIN "ref_files/${file}_merged.xml" "${file}_merged_2.xml"                                             2>&1 | tee -a test_merge.log
  compare_result=${PIPESTATUS[0]}
  if [ "$compare_result" -ne "0" ]; then
    success=0
    failed_tests="$failed_tests 2:$file"
  fi
  echo                                                                                                                | tee -a test_merge.log
  rm -rf "${file}_folder" # delete new folder
done

# command to test the merging of files b and a:
for file in $files; do
  echo "================================="                                                                            | tee -a test_merge.log
  echo "3. Merge test for $file"                                                                                      | tee -a test_merge.log
  echo                                                                                                                | tee -a test_merge.log
  python3 ../../../tools/coverage_merge/merge.py --merge_to_db=${file}_merged_3.xml ${file}_b.xml ${file}_a.xml  2>&1 | tee -a test_merge.log
  echo                                                                                                                | tee -a test_merge.log
  echo "Differences"                                                                                                  | tee -a test_merge.log
  # Calling the comparison tool: Expected XML first, then the generated XML
  $COMPARE_BIN "ref_files/${file}_merged.xml" "${file}_merged_3.xml"                                             2>&1 | tee -a test_merge.log
  compare_result=${PIPESTATUS[0]}
  if [ "$compare_result" -ne "0" ]; then
    success=0
    failed_tests="$failed_tests 3:$file"
  fi
  echo                                                                                                                | tee -a test_merge.log
done

# summary
echo "================================="                                                                              | tee -a test_merge.log
if [ "$success" -eq "1" ]; then
  echo "All tests passed."                                                                                            | tee -a test_merge.log
else
  echo "The following tests failed:"                                                                                  | tee -a test_merge.log
  for failed_test in $failed_tests; do
    echo "  $failed_test"                                                                                             | tee -a test_merge.log
  done
fi
echo "================================="                                                                              | tee -a test_merge.log
echo "End of merge testing"                                                                                           | tee -a test_merge.log
echo "================================="                                                                              | tee -a test_merge.log

