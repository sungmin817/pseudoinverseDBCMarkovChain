project(PseudoInverseToGithub)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++11 -ffast-math -Wall -Wextra -W -Wno-unused-local-typedefs")
set(BOOST_DIRECTORY "C:/MinGW/include")

include_directories(.)
include_directories(${BOOST_DIRECTORY})
