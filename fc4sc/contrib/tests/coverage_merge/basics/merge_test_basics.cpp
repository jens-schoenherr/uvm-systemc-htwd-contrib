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
#include <cstdint>
#include "wo_cvp.h"

#include "uint8_cvp.h"
#include "bool_uint8_cvp.h"
#include "char_cvp.h"
#include "bool_cvp.h"
#include "uint32_cvp.h"
#include "int_cvp.h"
#include "several_data_types_cvp.h"
#include "without_cross_test_cvp.h"
#include "cross_without_bin_cvp.h"


int main()
{
  std::cout << "Generating xml files for coverage." << std::endl;

  generate_wo_cvp();

  generate_uint8_cvp();         //test 1
  generate_bool_uint8_cvp();    //test 2
  generate_char_cvp();          //test 3
  generate_bool_cvp();          //test 4
  generate_uint32_cvp();        //test 5
  generate_int_cvp();           //test 6
  generate_several_data_types_cvp();  //test 7
  generate_without_cross_test_cvp();  //test 8
  generate_cross_without_bin_cvp();   //test 9

}
