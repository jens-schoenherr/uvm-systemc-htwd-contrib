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
   Authors  : Georg Christian Moosdorf, HTW Dresden, 2026-10-30
              Jens Schoenherr,          HTW Dresden, 2026-10-30
******************************************************************************/

#include "xml_compare.h"
#include <iostream>
#include <fstream>

// Read file line by line, skip \r (aka CR = 0x0d)
bool xml_compare::read_file(const std::string& filename, std::string& content)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cout << "[Error] Cannot open file: " << filename << std::endl;
    return false;
  }

  char c;
  while (file.get(c))
  {
    if (c != '\r')
    {
      content += c;
    }
  }
  file.close();
  trim_right(content);

  return true;
}

// remove whitespaces including line endings at end of file
void xml_compare::trim_right(std::string& str)
{
  while (!str.empty() && (str.back() == '\n' || str.back() == '\r' || str.back() == ' ' || str.back() == '\t'))
  {
    str.pop_back();
  }
}

bool xml_compare::compare_files(const std::string &expected, const std::string &actual)
{
  unsigned int e = 0; // Index for expected
  unsigned int a = 0; // Index for actual

  bool has_differences = false;

  char temp_char;

  while (a < actual.length() || (e < expected.length() && expected[e] == '*'))
  {
    // Case 1: Wildcard found
    if (e < expected.length() && expected[e] == '*')
    {
      temp_char = expected[e+1];
      while (temp_char != actual[a])
      {
        a++;
      }
      e++;
    }
    // Case 2: Exact character match
    else if (e < expected.length() && a < actual.length() && expected[e] == actual[a])
    {
      e++;
      a++;
    }
    // Case 3: Real mismatch found
    else
    {
       has_differences = true;
       int line = 1, col = 1;
       for (unsigned int i = 0; i < e ; i++)
       {
         if (expected[i] == '\n') { line++; col = 1; }
         else { col++; }
       }

       std::cout << "[Diff] Mismatch at line " << line << ", column " << col << "!" << std::endl;
       if (a < actual.length() && e < expected.length())
       {
         std::cout << "  Expected: '" << expected[e] << "'" << std::endl;
         std::cout << "  Found   : '" << actual[a]   << "'" << std::endl;
       } else {
         std::cout << "  Reason: Unexpected end of line or extra characters." << std::endl;
       }

       // Sync to next character to prevent loop lock
       if (a < actual.length())   a++;
       if (e < expected.length()) e++;
     }
  }

  if (e != expected.length() || a != actual.length())
  {
    has_differences = true;
    std::cout << "[Diff] File length or structure mismatch at the end of the file!" << std::endl;
  }

  return !has_differences;
}

bool xml_compare::compare(const std::string& expected_file, const std::string& actual_file)
{
  std::string expected;
  std::string actual;
  if (!read_file(expected_file, expected)) return false;
  if (!read_file(actual_file,   actual))   return false;
  return compare_files(expected, actual);
}

