cmake_minimum_required(VERSION 3.13)
project(FIMA)
set(CMAKE_CXX_STANDARD 23)

include(./scripts/cmake/files.cmake)

find_package(ftxui REQUIRED)

add_executable(fima ${SOURCE_FILES})

target_link_libraries(fima
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
