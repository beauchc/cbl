
add_executable(test_cbl test_streamable.cpp)
target_link_libraries(test_cbl cbl gtest gtest_main)

add_test(NAME test_cbl COMMAND test_cbl)
