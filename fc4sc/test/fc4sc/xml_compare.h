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

#include <string>

class xml_compare
{
  static bool read_file(const std::string& filename, std::string& content);
  static void trim_right(std::string& str);
  static bool compare_files(const std::string &expected, const std::string &actual);

public:
  // Compare the text file named by actual_file with the file named by expected_file.
  // The expected file may contain * as wildcards.
  static bool compare(const std::string& expected_file, const std::string& actual_file);
};

