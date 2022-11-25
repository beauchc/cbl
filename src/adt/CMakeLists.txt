add_library(cbl_adt INTERFACE)
target_include_directories(cbl_adt INTERFACE ${CMAKE_SOURCE_DIR}/include/adt)
target_link_libraries(cbl_adt INTERFACE cbl)
