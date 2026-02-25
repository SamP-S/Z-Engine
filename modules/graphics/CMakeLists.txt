message(STATUS "Building zen_graphics")

# set opengl config
if (POLICY CMP0072)
  cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

# zen graphics
set(GRAPHICS_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(GRAPHICS_PRIVATE_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src")
file(GLOB_RECURSE GRAPHICS_SOURCES "src/**.cpp")

add_library(zen_graphics STATIC ${GRAPHICS_SOURCES})
target_include_directories(zen_graphics PUBLIC ${GRAPHICS_INCLUDES})
target_include_directories(zen_graphics PRIVATE ${GRAPHICS_PRIVATE_INCLUDES})
target_link_libraries(zen_graphics PUBLIC zen::core)

# opengl dependencies
find_package(OpenGL REQUIRED)
target_include_directories(zen_graphics PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(zen_graphics PRIVATE ${OPENGL_LIBRARIES})

# glew dependencies
find_package(GLEW REQUIRED)
target_include_directories(zen_graphics PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(zen_graphics PRIVATE ${GLEW_LIBRARIES})

# alias project to match C++ namespace
add_library(zen::graphics ALIAS zen_graphics)

# examples
if (BUILD_EXAMPLES)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
endif()
