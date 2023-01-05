set(experiments
    dispatch_apply
    iaccessor
    any
    rtti
)

function(add_exp_dir_test dirname)
    set(target_name exp_${dirname})
    file(GLOB SOURCES "${dirname}/*.cpp")
    add_executable(${target_name} ${SOURCES})
    target_link_libraries(${target_name} cbl gtest gtest_main)
    add_test(NAME ${target_name} COMMAND ${target_name})
    set_property(TEST ${target_name} PROPERTY LABELS "exp")
endfunction()

foreach(exp ${experiments})
    add_exp_dir_test(${exp})
endforeach()
