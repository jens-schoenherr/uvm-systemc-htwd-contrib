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
   Authors  : Georg Christian Moosdorf, HTW Dresden, 2026-11-16
              Jens Schoenherr,          HTW Dresden, 2026-11-16
******************************************************************************/

#include <iostream>
#include "xml_compare.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: ./file_compare expected.xml actual.xml" << std::endl;
        return 2;
    }

  /*  XmlCompare x;
    if (x.compare(argv[1], argv[2]))*/
    if (xml_compare::compare(argv[1], argv[2]))
    {
        std::cout << "Files match successfully." << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Files differ or an error occurred." << std::endl;
        return 1;
    }
}
