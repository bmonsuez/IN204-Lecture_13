cmake_minimum_required(VERSION 3.28)
project(part1 CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(q1 main.q1.cpp)

target_sources(q1 
  PUBLIC 
  FILE_SET cxx_modules TYPE CXX_MODULES 
  FILES math.q1.ixx)

add_executable(q2 main.q2.cpp)

target_sources(q2
  PUBLIC 
  FILE_SET cxx_modules TYPE CXX_MODULES 
  FILES math.q2.ixx)

add_executable(q3 main.q3.cpp)

target_sources(q3
  PUBLIC 
  FILE_SET cxx_modules TYPE CXX_MODULES 
  FILES math.q3.ixx)
