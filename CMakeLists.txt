cmake_minimum_required(VERSION 3.13)
project(FIMA)
set(CMAKE_CXX_STANDARD 23)

include(./scripts/cmake/files.cmake)

add_executable(fima ${SOURCE_FILES})

