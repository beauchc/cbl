function(cbl_gtest_unittests target)
    set(prefix D)
    set(flags)
    set(singles)
    set(multis LINK_LIBS SOURCES INCLUDES)
    cmake_parse_arguments(${prefix} "${flags}" "${singles}" "${multis}" ${ARGN})

    add_executable(${target} ${D_SOURCES})
    target_include_directories(${target} PRIVATE ${D_INCLUDES})
    target_link_libraries(${target} ${D_LINK_LIBS} gtest gtest_main)
    add_test(NAME ${target} COMMAND ${target})
endfunction()
