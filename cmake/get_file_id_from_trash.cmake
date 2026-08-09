#! get_file_id_from_trash : This functions gets an ID from the trash of FIMA
function(get_file_id_from_trash id)
    file(GLOB files "$ENV{HOME}/.local/share/fima/trash/files/*")

    if(NOT files)
        message(WARNING "No files found in the trash")

        return()
    endif()

    list(GET files 0 file)

    get_filename_component(file_name "${file}" NAME)

    string(REGEX MATCH "^[0-9]*_[0-9]*_[0-9]*" result "${file_name}")

    set(id
        "${result}"
        PARENT_SCOPE
    )
endfunction()
