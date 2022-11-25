add_subdirectory(dispatch_apply)

function(add_exp_test filename)
    string(LENGTH ${filename} len)
    math(EXPR len ${len}-4)
    string(SUBSTRING ${filename} 0 ${len} target_name)

    set(target_name exp_${target_name})
    add_executable(${target_name} ${filename} ${ARGN})
    target_link_libraries(${target_name} gtest gtest_main)
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

add_exp_test(iaccessor.cpp iapply.cpp)
