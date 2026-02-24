cmake_minimum_required(VERSION 3.22)
project(zengine_core)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# spdlog
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog")

message(STATUS "zengine_core")

# core library
set(CORE_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
message(STATUS "INCLUDES: ${CORE_INCLUDES}")

# glob all .cpp sources
file(GLOB_RECURSE CORE_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp"
)
message(STATUS "SOURCES: ${CORE_SOURCES}")

# create the library
add_library(zen_core ${CORE_SOURCES})

# add includes
target_include_directories(zen_core PUBLIC ${CORE_INCLUDES})

# link spdlog
target_link_libraries(zen_core PUBLIC spdlog::spdlog)

# alias project to match C++ namespace
add_library(zengine::core ALIAS zen_core)

# testing
if(PROJECT_NAME STREQUAL "zengine_core")
    enable_testing()
    file(GLOB_RECURSE CORE_TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/**.cpp"
    )
    add_executable(zen_core_test ${CORE_TEST_SOURCES})
    target_link_libraries(zen_core_test PRIVATE zengine::core)
    add_test(NAME zen_core_test COMMAND zen_core_test)
    message(STATUS "zengine_core: built tests")
endif()
