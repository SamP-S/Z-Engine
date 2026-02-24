cmake_minimum_required(VERSION 3.22)
project(Zengine)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# git submodule
find_package(Git REQUIRED)
execute_process(
    COMMAND ${GIT_EXECUTABLE}
    submodule update --init --recursive
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# zengine_core
add_subdirectory(core)

add_executable(test "tests/main.cpp")
target_link_libraries(test PRIVATE zengine::core)
