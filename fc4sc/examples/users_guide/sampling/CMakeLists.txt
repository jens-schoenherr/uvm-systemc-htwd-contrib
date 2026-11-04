cmake_minimum_required(VERSION 3.28)

#set(CMAKE_CXX_COMPILER          g++)
set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)
#set(CMAKE_VERBOSE_MAKEFILE       ON)

project(sampling LANGUAGES CXX)

add_executable(sampling)

target_sources(sampling PRIVATE
  sampling_example.cpp
  bins.cpp
  )

target_include_directories(sampling PRIVATE
  ../../../includes
  )

