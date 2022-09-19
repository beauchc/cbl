function(init_boost)
    set(BOOST_DIR ${CMAKE_CURRENT_LIST_DIR}/../external/boost)
    if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
        set(BOOST_DIR ${BOOST_DIR}/Debug)
    else()
        set(BOOST_DIR ${BOOST_DIR}/Release)
    endif()

    add_library(cbl__boost INTERFACE)
    target_include_directories(cbl__boost INTERFACE ${BOOST_DIR}/include)
    add_library(cbl::boost ALIAS cbl__boost)
endfunction()

init_boost()
