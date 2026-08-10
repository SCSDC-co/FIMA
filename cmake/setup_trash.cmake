#! setup_trash : This function is used to setup the trash with hard coded values
function(setup_trash)
    set(trash_dir "$ENV{HOME}/.local/share/fima/trash")

    file(MAKE_DIRECTORY "${trash_dir}/files")
    file(MAKE_DIRECTORY "${trash_dir}/trashinfo")

    file(TOUCH "${trash_dir}/files/20260806_195324_0_test.txt")

    file(
        WRITE "${trash_dir}/trashinfo/20260806_195324_0_test.txt.trashinfo"
        "[metadata]\npath = \"$ENV{HOME}/test.txt\"\ndeleted_at = 2026-08-06T19:53:24"
    )

    file(TOUCH "${trash_dir}/files/20260806_195324_1_test1.txt")

    file(
        WRITE "${trash_dir}/trashinfo/20260806_195324_1_test1.txt.trashinfo"
        "[metadata]\npath = \"$ENV{HOME}/test1.txt\"\ndeleted_at = 2026-08-06T19:53:24"
    )

    message(STATUS "The trash has been setupped correctly.")
endfunction()
