cmake_minimum_required(VERSION 3.31)
project(FIMA)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(./scripts/cmake/files.cmake)

include(FetchContent)

FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v6.1.9
)

FetchContent_MakeAvailable(ftxui)

add_executable(fima ${SOURCE_FILES})

target_link_libraries(fima
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
