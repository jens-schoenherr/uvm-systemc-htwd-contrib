cmake_minimum_required(VERSION 3.28)

#set(CMAKE_CXX_COMPILER          g++)
set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)
#set(CMAKE_VERBOSE_MAKEFILE       ON)

project(gui_test_basics LANGUAGES CXX)

add_executable(gui_test_basics)

target_sources(gui_test_basics PRIVATE
		char_cvp.cpp
		gui_test_basics.cpp
		uint8_cvp.cpp
		wo_cvp.cpp
  )

target_include_directories(gui_test_basics PRIVATE
  ../../../../includes
  )

