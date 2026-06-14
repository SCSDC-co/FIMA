cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    fima
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "FIMA - Fast, Incredible, Minimal & Awesome File Manager"
)

include(CTest)

find_package(nlohmann_json REQUIRED)
find_package(ftxui REQUIRED)
find_package(libgit2 REQUIRED)
find_package(libzippp REQUIRED)
find_package(termcolor REQUIRED)
find_package(CLI11 REQUIRED)

add_subdirectory(./src/)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(
    fima_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
    fima_lib
    PUBLIC
        libzippp::libzippp
        termcolor::termcolor
        ftxui::ftxui
        nlohmann_json::nlohmann_json
        libgit2::libgit2
        CLI11::CLI11
)

target_link_libraries(${PROJECT_NAME} PRIVATE fima_lib)

if(NOT MSVC)
    target_compile_options(example PUBLIC -Wall -pedantic)
endif()

include(./cmake/tests.cmake)
