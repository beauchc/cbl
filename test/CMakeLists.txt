
function(add_libs_unittests libpath)
    get_filename_component(libname ${libpath} DIRECTORY)
    get_filename_component(libname ${libname} NAME)
    if ( NOT ${libname} STREQUAL "cbl" )
        set(libname cbl_${libname})
    endif()
    message(STATUS "Configuring tests for lib '${libname}'")

    file(GLOB dirs "${libpath}/*")
    foreach(dir ${dirs})
        if ( NOT IS_DIRECTORY ${dir} )
            continue()
        endif()
        file(GLOB sources RELATIVE ${dir} "${dir}/*.h")

        get_filename_component(dir ${dir} NAME)
        list(TRANSFORM sources REPLACE ".h" ".cpp")
        list(TRANSFORM sources PREPEND "${dir}/test_")

        set(target cbl_${dir}_unittests)
        message(STATUS "    Configuring unittests '${target}'")
        foreach(source ${sources})
            message(STATUS "        ${source}")
        endforeach()
        list(TRANSFORM sources PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

        cbl_gtest_unittests(${target}
            SOURCES ${sources}
            LINK_LIBS ${libname})
    endforeach()
endfunction()

set(CBL_INCLUDE ${CMAKE_SOURCE_DIR}/include)
file(GLOB libs "${CBL_INCLUDE}/*/*")

foreach(lib ${libs})
    add_libs_unittests(${lib})
endforeach()

add_subdirectory(exp)
