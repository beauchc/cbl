add_library(cbl_debug SHARED
    string_cursors.cpp
)
target_include_directories(cbl_debug PUBLIC ${CMAKE_SOURCE_DIR}/include/debug)
target_link_libraries(cbl_debug PUBLIC cbl)
