
function(add_libs_unittests libpath)
    get_filename_component(libname ${libpath} DIRECTORY)
    get_filename_component(libname ${libname} NAME)

    file(GLOB include_dirs "${libpath}/*")
    foreach(incl_dir ${include_dirs})
        if ( NOT IS_DIRECTORY ${incl_dir} )
            continue()
        endif()
        get_filename_component(dir ${incl_dir} NAME)
        set(test_dir "${CMAKE_CURRENT_SOURCE_DIR}/${dir}")

        file(GLOB headers RELATIVE ${incl_dir} "${incl_dir}/*.h")
        file(GLOB sources RELATIVE ${test_dir} "${test_dir}/*.cpp")

        set(target cbl_${dir}_unittests)
        set(s "    ")
        message(STATUS "Configuring unittests '${target}'")
        foreach(source ${sources})
            message(STATUS "${s}${source}")
        endforeach()

        foreach(header ${headers})
            set(test_file ${header})
            set(header "${libname}/${dir}/${header}")
            string(REPLACE ".h" ".cpp" test_file ${test_file})
            string(PREPEND test_file "${test_dir}/test_")
            if ( NOT EXISTS ${test_file} )
                message(STATUS "${s}WARNING: Missing test for ${header}")
            endif()
        endforeach()
        list(TRANSFORM sources PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/${dir}/")

        if ( NOT ${libname} STREQUAL "cbl" )
            set(libname cbl_${libname})
        endif()

        cbl_gtest_unittests(${target}
            SOURCES ${sources}
            LINK_LIBS ${libname})
        message(STATUS "")
    endforeach()
endfunction()

set(CBL_INCLUDE ${CMAKE_SOURCE_DIR}/include)
file(GLOB libs "${CBL_INCLUDE}/*/*")

foreach(lib ${libs})
    add_libs_unittests(${lib})
endforeach()

add_subdirectory(exp)
