add_library(cbl INTERFACE)
target_include_directories(cbl INTERFACE ${CMAKE_SOURCE_DIR}/include/cbl)

add_subdirectory(adt)
add_subdirectory(debug)
