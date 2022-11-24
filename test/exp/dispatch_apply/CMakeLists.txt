add_executable(exp_dispatch_apply
    dispatch_apply.cpp
)
target_link_libraries(exp_dispatch_apply gtest gtest_main)

add_test(NAME exp_dispatch_apply COMMAND exp_dispatch_apply)
