#! get_file_id_from_trash : This functions gets an ID from the trash of FIMA
function(get_ids_from_trash ids)
    file(GLOB files "$ENV{HOME}/.local/share/fima/trash/files/*")

    if(NOT files)
        message(WARNING "No files found in the trash")

        return()
    endif()

    set(result_ids)

    foreach(file IN LISTS files)
        get_filename_component(file_name "${file}" NAME)

        string(REGEX MATCH "^[0-9]*_[0-9]*_[0-9]*" result "${file_name}")

        if(result)
            list(APPEND result_ids "${result}")
        endif()
    endforeach()

    set(${ids}
        ${result_ids}
        PARENT_SCOPE
    )
endfunction()
