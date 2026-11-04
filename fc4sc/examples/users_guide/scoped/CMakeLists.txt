cmake_minimum_required(VERSION 3.28)

#set(CMAKE_CXX_COMPILER          g++)
set(CMAKE_CXX_STANDARD           17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS        OFF)
#set(CMAKE_VERBOSE_MAKEFILE       ON)

project(scoped LANGUAGES CXX)

add_executable(scoped)

target_sources(scoped PRIVATE
		context_info.cpp
		dynamic_scope.cpp
		enum_cvp.cpp
		hierarchical_scope.cpp
		minimal_cvp.cpp
		scoped_example.cpp
		uint8_cvp.cpp
		wo_cvp.cpp
  )

target_include_directories(scoped PRIVATE
  ../../../includes
  )

