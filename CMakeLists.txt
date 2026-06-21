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

find_package(ftxui REQUIRED)
find_package(libgit2 REQUIRED)
find_package(libzippp REQUIRED)
find_package(termcolor REQUIRED)
find_package(p-ranav-glob REQUIRED)
find_package(tomlplusplus REQUIRED)
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
        libgit2::libgit2
        p-ranav-glob::p-ranav-glob
        tomlplusplus::tomlplusplus
        CLI11::CLI11
)

target_link_libraries(${PROJECT_NAME} PRIVATE fima_lib)

if(NOT MSVC)
    target_compile_options(
        ${PROJECT_NAME}
        PUBLIC
            -Wall
            -Wextra
            -Wpedantic
            -Wreorder
            -Wunused
            -Wshadow
            -Werror=return-type
            -Weffc++
            -Wvla
            -Wconversion
            -Wunused-result
            -Wswitch
            -Wswitch-default
            -Wswitch-enum
    )
else()
    target_compile_options(
        ${PROJECT_NAME}
        PUBLIC /W4 /WX /permissive- /Zc:__cplusplus /diagnostics:caret
    )
endif()

include(./cmake/tests.cmake)
