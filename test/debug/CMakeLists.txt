
add_executable(test_cbl_debug
    test_string_cursors.cpp
)
target_link_libraries(test_cbl_debug cbl_debug gtest gtest_main)

add_test(NAME test_cbl_debug COMMAND test_cbl_debug)
