enable_testing()

include(CTest)
include(setup_trash)

# code doverage config
add_library(coverage_config INTERFACE)

option(FIMA_CODE_COVERAGE "Enable coverage reporting" OFF)

if(FIMA_CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(
        coverage_config
        INTERFACE -O0 # no optimization
                  -g # generate debug info
                  --coverage # sets all required flags
    )

    target_link_options(coverage_config INTERFACE --coverage)

    target_link_libraries(fima_lib PUBLIC coverage_config)
    target_link_libraries(${PROJECT_NAME} PUBLIC coverage_config)
endif()

option(FIMA_BUILD_TESTS "Builds the test cases" OFF)

if(FIMA_BUILD_TESTS)
    setup_trash()

    add_test(NAME version COMMAND ${PROJECT_NAME} -v)
    add_test(NAME version_subcmd COMMAND ${PROJECT_NAME} version)
    add_test(NAME ls COMMAND ${PROJECT_NAME} .. ls)
    add_test(NAME ls_all_options COMMAND ${PROJECT_NAME} ../tests ls -ialvH)
    add_test(NAME ls_one_line COMMAND ${PROJECT_NAME} .. ls -1)
    add_test(NAME tree COMMAND ${PROJECT_NAME} .. tree)
    add_test(NAME mk COMMAND ${PROJECT_NAME} mk -d tests -f tests/test1.txt
                             tests/test2.txt
    )
    add_test(NAME cp COMMAND ${PROJECT_NAME} cp tests/test2.txt tests/test3.txt)
    add_test(NAME cp_dir COMMAND ${PROJECT_NAME} cp tests tests_cp)
    add_test(NAME mv COMMAND ${PROJECT_NAME} mv tests/test3.txt test.txt)
    add_test(NAME rm COMMAND ${PROJECT_NAME} rm tests/test2.txt tests tests_cp
                             test.txt -rvt
    )
    add_test(NAME perms COMMAND ${PROJECT_NAME} perms ../tests/all_perms.txt)
    add_test(NAME cloc COMMAND ${PROJECT_NAME} .. cloc)
    add_test(NAME cloc_all_options COMMAND ${PROJECT_NAME} .. cloc -i "*.hpp"
                                           -S files -qG
    )
    add_test(NAME cloc_show_languages COMMAND ${PROJECT_NAME} .. cloc -s)
    add_test(NAME info COMMAND ${PROJECT_NAME} ../src info -v)
    add_test(NAME info_file COMMAND ${PROJECT_NAME} info ../conanfile.py)
    add_test(NAME info_git COMMAND ${PROJECT_NAME} info -grt)
    add_test(NAME mk_for_zip COMMAND ${PROJECT_NAME} mk -d tests -f
                                     tests/test1.txt tests/test2.txt
    )
    add_test(NAME zip COMMAND ${PROJECT_NAME} zip tests -o tests.zip)
    add_test(NAME create_item_to_add_items_to_zip COMMAND ${PROJECT_NAME} mk -f
                                                          test.txt
    )
    add_test(NAME add_items_to_zip COMMAND ${PROJECT_NAME} zip test.txt -o
                                           tests.zip
    )
    add_test(NAME unzip COMMAND ${PROJECT_NAME} unzip tests.zip -o unzip_tests)
    add_test(NAME rm_for_unzip COMMAND ${PROJECT_NAME} rm tests -rvt)

    add_test(NAME mk_for_trash COMMAND ${PROJECT_NAME} mk -f file.txt file2.txt
                                       file3.txt
    )
    add_test(NAME rm_for_trash COMMAND ${PROJECT_NAME} rm file.txt file2.txt
                                       file3.txt
    )

    add_test(NAME trash_list COMMAND ${PROJECT_NAME} trash list)
    add_test(NAME trash_list_plain COMMAND ${PROJECT_NAME} trash list --plain)

    add_test(NAME trash_restore COMMAND ${PROJECT_NAME} trash restore
                                        20260806_195324_0
    )
    add_test(NAME trash_remove COMMAND ${PROJECT_NAME} trash remove
                                       20260806_195324_0
    )

    add_test(NAME trash_empty COMMAND ${PROJECT_NAME} trash empty -y)

    add_test(NAME tui COMMAND ${PROJECT_NAME})

    add_test(NAME default_theme COMMAND ${PROJECT_NAME} --dump-default-theme)
    add_test(NAME default_config COMMAND ${PROJECT_NAME} --dump-default-config)
endif()
