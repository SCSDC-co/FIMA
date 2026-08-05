execute_process(
    COMMAND ${PROJECT_NAME} trash list --plain OUTPUT_VARIABLE trash_output
)

string(
    REGEX MATCHALL
          "[0-9]+_[0-9]+_[0-9]+"
          trash_ids
          "${trash_output}"
)

foreach(id IN LISTS trash_ids)
    execute_process(
        COMMAND ${PROJECT_NAME} trash restore ${id} RESULT_VARIABLE result
    )
endforeach()
