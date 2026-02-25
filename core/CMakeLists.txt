message(STATUS "Building zen_core")

# external dependencies
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/la")

# core library
set(CORE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
file(GLOB_RECURSE CORE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp")
add_library(zen_core STATIC ${CORE_SOURCES})
target_include_directories(zen_core PUBLIC ${CORE_INCLUDES})

# link spdlog
target_link_libraries(zen_core PUBLIC spdlog::spdlog)

# link la
target_link_libraries(zen_core PUBLIC la)

# alias project to match C++ namespace
add_library(zen::core ALIAS zen_core)

# testing and examples
if (BUILD_EXAMPLES)
    message(STATUS "Building zen_core examples")
    file(GLOB_RECURSE CORE_EXAMPLES_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/examples/**.cpp"
    )
    add_executable(zen_core_example ${CORE_EXAMPLES_SOURCES})
    target_link_libraries(zen_core_example PUBLIC zen::core)
endif()
