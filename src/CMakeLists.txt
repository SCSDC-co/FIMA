file(
    GLOB_RECURSE
    fima_files
    ./*.cpp
)

add_library(fima_lib ${fima_files})
