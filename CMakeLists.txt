cmake_minimum_required(VERSION 3.12)

project(cbl)

# Enable c++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  add_compile_definitions("_GLIBCXX_DEBUG")
endif()

if(MSVC)
  add_definitions(/MP)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
if(WIN32)
set(CMAKE_CXX_FLAGS_DEBUG "/ZI /Od")
set(CMAKE_CXX_FLAGS_RELEASE "/O2")
else(WIN32)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")
endif(WIN32)

if(${CMAKE_BUILD_TYPE} STREQUAL Coverage)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} --coverage")
add_definitions(-DNDEBUG)
endif()

add_subdirectory(src)

if (NOT DEFINED ${CBL_DISABLE_TESTING})
    enable_testing()
    add_subdirectory(external/googletest/googletest)
    add_subdirectory(test)
endif()
