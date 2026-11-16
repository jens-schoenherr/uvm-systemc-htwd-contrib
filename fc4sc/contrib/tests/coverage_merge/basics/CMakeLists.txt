cmake_minimum_required(VERSION 3.28)

#set(CMAKE_CXX_COMPILER          g++)
set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)
#set(CMAKE_VERBOSE_MAKEFILE       ON)

project(merge_test_basics LANGUAGES CXX)

add_executable(merge_test_basics)

target_sources(merge_test_basics PRIVATE
    bool_cvp.cpp
    bool_uint8_cvp.cpp
    char_cvp.cpp
    cross_without_bin_cvp.cpp
    int_cvp.cpp
    merge_test_basics.cpp
    several_data_types_cvp.cpp
    uint32_cvp.cpp
    uint8_cvp.cpp
    without_cross_test_cvp.cpp
    wo_cvp.cpp
  )

target_include_directories(merge_test_basics PRIVATE
  ../../../../includes
  )

add_executable(xml_file_compare)

target_sources(xml_file_compare PRIVATE
    xml_file_compare.cpp
    ../../../../test/fc4sc/xml_compare.cpp
  )

target_include_directories(xml_file_compare PRIVATE
  ../../../../includes
  ../../../../test/fc4sc
  )

