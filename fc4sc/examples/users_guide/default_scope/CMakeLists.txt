cmake_minimum_required(VERSION 3.28)

#set(CMAKE_CXX_COMPILER          g++)
set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)
#set(CMAKE_VERBOSE_MAKEFILE       ON)

project(default_scope LANGUAGES CXX)

add_executable(default_scope)

target_sources(default_scope PRIVATE
  default_scope_example.cpp
  uint8_cvp.cpp
  wo_cvp.cpp
  )

target_include_directories(default_scope PRIVATE
  ../../../includes
  )

