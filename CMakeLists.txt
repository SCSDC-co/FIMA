cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GitVersionDetect)

set(PROJECT_DESCRIPTION "Fast, Incredible, Minimal & Awesome File Manager")

project(
    fima
    LANGUAGES CXX
    VERSION
        ${GITVERSIONDETECT_VERSION_MAJOR}.${GITVERSIONDETECT_VERSION_MINOR}.${GITVERSIONDETECT_VERSION_PATCH}
    DESCRIPTION ${PROJECT_DESCRIPTION}
)

find_package(ftxui REQUIRED)
find_package(libgit2 REQUIRED)
find_package(libzippp REQUIRED)
find_package(rang REQUIRED)
find_package(p-ranav-glob REQUIRED)
find_package(tomlplusplus REQUIRED)
find_package(CLI11 REQUIRED)

add_subdirectory(./src/)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(
    fima_lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
                    $<INSTALL_INTERFACE:include/>
)

target_link_libraries(
    fima_lib
    PUBLIC libzippp::libzippp
           rang::rang
           ftxui::ftxui
           libgit2::libgit2
           p-ranav-glob::p-ranav-glob
           tomlplusplus::tomlplusplus
           CLI11::CLI11
)

if(WIN32)
    message(STATUS "Building for windows, libmagic will not be included")

    target_link_libraries(fima_lib PRIVATE Advapi32)
else()
    find_package(libmagic REQUIRED)

    message(STATUS "Not building for windows, linking libmagic")

    target_link_libraries(fima_lib PUBLIC libmagic::libmagic)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE fima_lib)

if(NOT MSVC)
    target_compile_options(
        ${PROJECT_NAME}
        PUBLIC -Wall
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
    target_link_options(
        ${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc
    )
else()
    target_compile_options(
        ${PROJECT_NAME}
        PUBLIC /W4
               /WX
               /permissive-
               /Zc:__cplusplus
               /diagnostics:caret
    )
endif()

target_compile_definitions(
    fima_lib
    PUBLIC VERSION_MAJOR=${GITVERSIONDETECT_VERSION_MAJOR}
           VERSION_MINOR=${GITVERSIONDETECT_VERSION_MINOR}
           VERSION_PATCH=${GITVERSIONDETECT_VERSION_PATCH}
           VERSION_STRING="${GITVERSIONDETECT_VERSION_PRETTY}"
)

include(GNUInstallDirs)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(NOT WIN32)
    if(NOT FIMA_MAGIC_DATABASE)
        message(FATAL_ERROR "FIMA_MAGIC_DATABASE was not provided by Conan")
    endif()

    install(FILES "${FIMA_MAGIC_DATABASE}"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/fima"
    )

    set(FIMA_MAGIC_DATABASE_INSTALL_PATH
        "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATADIR}/fima/magic.mgc"
    )

    target_compile_definitions(
        fima_lib
        PUBLIC FIMA_MAGIC_DATABASE="${FIMA_MAGIC_DATABASE_INSTALL_PATH}"
    )
endif()

include(tests)
include(deploy)
