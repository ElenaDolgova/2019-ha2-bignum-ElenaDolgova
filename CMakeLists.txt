cmake_minimum_required(VERSION 3.13)
project(bignum)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX") # CMake already contains W3 in its flags
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

add_executable(huffman smoke_test.cpp bignum.hpp)
#add_executable(huffman bignum.hpp)