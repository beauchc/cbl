file(GLOB SOURCES "test*.cpp")

set(target_name cbl_adt_unittests)
add_executable(${target_name} ${SOURCES})
target_link_libraries(${target_name} cbl_adt gtest gtest_main)
add_test(NAME ${target_name} COMMAND ${target_name})
set_property(TEST ${target_name} PROPERTY LABELS "adt")
