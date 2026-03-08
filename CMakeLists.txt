cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(fima)

find_package(nlohmann_json REQUIRED)
find_package(ftxui REQUIRED)
find_package(libgit2 REQUIRED)
find_package(gzip-hpp REQUIRED)

include(./scripts/cmake/files.cmake)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(
    fima_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
        $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
        fima_lib
        ftxui::ftxui
        nlohmann_json::nlohmann_json
        libgit2::libgit2
        gzip-hpp::gzip-hpp
)
