# Project configuration for zengine
cmake_minimum_required(VERSION 3.22)
project(zen LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)
set(CMAKE_BUILD_TYPE            Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")

# options
option(BUILD_CORE "Build core library" ON)
option(BUILD_MODULES "Build all modules" ON)
option(BUILD_ENGINE "Build engine targets" ON)
option(BUILD_PROJECTS "Build sample projects" ON)
option(BUILD_EXAMPLES "Build example apps" ON)

# options checks
if (NOT BUILD_CORE AND NOT BUILD_MODULES AND NOT BUILD_ENGINE AND NOT BUILD_PROJECTS)
    message(FATAL_ERROR "At least one of BUILD_CORE, BUILD_MODULES, BUILD_ENGINE, or BUILD_PROJECTS must be ON")
elseif (BUILD_MODULES AND (NOT BUILD_CORE))
    message(FATAL_ERROR "BUILD_MODULES requires BUILD_CORE to be ON")
elseif (BUILD_ENGINE AND (NOT BUILD_CORE OR NOT BUILD_MODULES))
    message(FATAL_ERROR "BUILD_ENGINE requires BUILD_CORE and BUILD_MODULES to be ON")
elseif (BUILD_PROJECTS AND (NOT BUILD_CORE OR NOT BUILD_MODULES OR NOT BUILD_ENGINE))
    message(FATAL_ERROR "BUILD_PROJECTS requires BUILD_CORE, BUILD_MODULES and BUILD_ENGINE to be ON")
endif()

# git submodule
find_package(Git REQUIRED)
execute_process(
    COMMAND ${GIT_EXECUTABLE}
    submodule update --init --recursive
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

# zen core
if (BUILD_CORE)
    add_subdirectory(core)
endif()

# zen modules
if (BUILD_MODULES)
    add_subdirectory(modules)
endif()

# zen engine
if (BUILD_ENGINE)
    message(STATUS "No engine to build yet")
endif()

# zen projects
if (BUILD_PROJECTS)
    message(STATUS "No projects to build yet")
endif()

# testing
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
