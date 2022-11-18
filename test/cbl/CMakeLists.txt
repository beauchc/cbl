
add_executable(test_cbl 
    test_streamable.cpp
    test_matrix.cpp
    test_partitions.cpp
    test_pool.cpp
    test_small_vector.cpp
    test_span.cpp
    test_debug_trace.cpp
)
target_link_libraries(test_cbl cbl gtest gtest_main)

add_test(NAME test_cbl COMMAND test_cbl)
