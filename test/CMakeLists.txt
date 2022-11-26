
function(cbl_add_unittests)
    file(GLOB SOURCES "*.cpp")
    cbl_gtest_unittests(cbl_unittests
        SOURCES ${SOURCES}
        LINK_LIBS cbl)

    file(GLOB DIRS "*/")

    foreach(dir ${DIRS})
        if ( NOT IS_DIRECTORY ${dir} )
            continue()
        endif()

        get_filename_component(dir ${dir} NAME)
        set(LINK_LIBS cbl)
        if ( ${dir} STREQUAL "exp" )
            continue()
        elseif ( ${dir} STREQUAL "debug" )
            set(LINK_LIBS cbl_debug)
        endif()

        file(GLOB SOURCES "${dir}/*.cpp")
        set(target cbl_${dir}_unittests)

        cbl_gtest_unittests(${target}
            SOURCES ${SOURCES}
            LINK_LIBS ${LINK_LIBS})
    endforeach()
endfunction()

cbl_add_unittests()
add_subdirectory(exp)
