file(GLOB sources "*/*.cpp")

add_library(cbl STATIC ${sources})
target_include_directories(cbl PUBLIC ${CMAKE_SOURCE_DIR}/include/cbl)
