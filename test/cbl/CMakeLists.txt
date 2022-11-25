file(GLOB SOURCES "test*.cpp")

set(target_name cbl_unittests)
add_executable(${target_name} ${SOURCES})
target_link_libraries(${target_name} cbl gtest gtest_main)
add_test(NAME ${target_name} COMMAND ${target_name})
