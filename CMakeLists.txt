cmake_minimum_required(VERSION 3.17)
project(cadical)

SET(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} ${CMAKE_SOURCE_DIR}/build/libcadical.a")

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(aaf-cf
        test/aaf/cf.cpp)
add_executable(aaf-adm
        test/aaf/adm.cpp)
add_executable(aaf-cmp
        test/aaf/cmp.cpp)
add_executable(aaf-prf
        test/aaf/prf.cpp)
add_executable(aaf-stb
        test/aaf/stb.cpp)
