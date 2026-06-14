enable_testing()

# code doverage config
add_library(coverage_config INTERFACE)

option(FIMA_CODE_COVERAGE "Enable coverage reporting" OFF)

if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(
        coverage_config
        INTERFACE
            -O0 # no optimization
            -g # generate debug info
            --coverage # sets all required flags
    )

    target_link_options(coverage_config INTERFACE --coverage)

    target_link_libraries(${PROJECT_NAME} PUBLIC coverage_config)
endif()

option(FIMA_BUILD_TESTS "Builds the test cases" OFF)

if(FIMA_BUILD_TESTS)
    add_test(NAME test_version COMMAND ${PROJECT_NAME} -v)
    add_test(NAME test_ls COMMAND ${PROJECT_NAME} .. ls)
    add_test(NAME test_tree COMMAND ${PROJECT_NAME} .. tree)
    add_test(
        NAME test_mk
        COMMAND ${PROJECT_NAME} mk -d tests -f tests/test1.txt tests/test2.txt
    )
    add_test(
        NAME test_cp
        COMMAND ${PROJECT_NAME} cp tests/test2.txt tests/test3.txt
    )
    add_test(NAME test_mv COMMAND ${PROJECT_NAME} mv tests/test3.txt ./test.txt)
    add_test(
        NAME test_rm
        COMMAND ${PROJECT_NAME} rm tests/test2.txt tests ./test.txt -r
    )
    add_test(NAME test_perms COMMAND ${PROJECT_NAME} perms ../conanfile.py)
    add_test(NAME test_cloc COMMAND ${PROJECT_NAME} .. cloc)
    add_test(NAME test_info COMMAND ${PROJECT_NAME} .. info -v)
    add_test(NAME test_info_file COMMAND ${PROJECT_NAME} info ../conanfile.py)
    add_test(NAME test_info_git COMMAND ${PROJECT_NAME} info -grt)
    add_test(
        NAME test_mk_for_zip
        COMMAND ${PROJECT_NAME} mk -d tests -f tests/test1.txt tests/test2.txt
    )
    add_test(
        NAME test_zip
        COMMAND ${PROJECT_NAME} zip tests -o tests.zip
    )
    add_test(
        NAME test_unzip
        COMMAND ${PROJECT_NAME} unzip tests.zip -o tests
    )
    add_test(NAME test_rm_for_unzip COMMAND ${PROJECT_NAME} rm tests -r)
endif()
