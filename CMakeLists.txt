cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GitVersionDetect)

set(PROJECT_VERSION_MAJOR ${GITVERSIONDETECT_VERSION_MAJOR})
set(PROJECT_VERSION_MINOR ${GITVERSIONDETECT_VERSION_MINOR})
set(PROJECT_VERSION_PATCH ${GITVERSIONDETECT_VERSION_PATCH})
set(PROJECT_VERSION ${GITVERSIONDETECT_VERSION})
set(PROJECT_DESCRIPTION "FIMA - Fast, Incredible, Minimal & Awesome File Manager")


project(
    fima
    LANGUAGES CXX
    VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}
    DESCRIPTION ${PROJECT_DESCRIPTION}
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
    target_compile_options(${PROJECT_NAME} PUBLIC -Wall -pedantic)
endif()

target_compile_definitions(fima_lib PUBLIC
  VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
  VERSION_MINOR=${PROJECT_VERSION_MINOR}
  VERSION_PATCH=${PROJECT_VERSION_PATCH}
  VERSION_STRING="${PROJECT_VERSION}-${GITVERSIONDETECT_VERSION_COMMIT_SHA}"
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

include(tests)
include(deploy)
