# shared library
set(SHARED_INCLUDES
	"${CMAKE_CURRENT_SOURCE_DIR}/external"
	"${CMAKE_CURRENT_SOURCE_DIR}/shared"
)
file(GLOB_RECURSE SHARED_SRC "${CMAKE_CURRENT_SOURCE_DIR}/shared/**.cpp")
add_library(shared ${SHARED_SRC})
target_include_directories(shared PUBLIC ${SHARED_INCLUDES})

# required libraries
find_package(OpenGL REQUIRED)
target_include_directories(shared PUBLIC ${OPENGL_INCLUDE_DIRS})
target_link_libraries(shared PUBLIC ${OPENGL_LIBRARIES})
message(STATUS "OpenGL included at ${OPENGL_INCLUDE_DIRS}")
message(STATUS "OpenGL libraries are ${OPENGL_LIBRARIES}")

find_package(SDL2 REQUIRED)
target_include_directories(shared PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(shared PUBLIC ${SDL2_LIBRARIES})
message(STATUS "SDL2 included at ${SDL2_INCLUDE_DIRS}")
message(STATUS "SDL2 libraries are ${SDL2_LIBRARIES}")

find_package(GLEW REQUIRED)
target_include_directories(shared PUBLIC ${GLEW_INCLUDE_DIRS})
target_link_libraries(shared PUBLIC ${GLEW_LIBRARIES})
message(STATUS "GLEW included at ${GLEW_INCLUDE_DIRS}")
message(STATUS "GLEW libraries are ${GLEW_LIBRARIES}")

# vgl library
target_link_libraries(shared PUBLIC zen::graphics)

# driver executable
set(DRIVER_INCLUDES
	"${CMAKE_CURRENT_SOURCE_DIR}/apps/00_cube"
	"${CMAKE_CURRENT_SOURCE_DIR}/apps/01_multithreaded"
)
file(GLOB_RECURSE APP_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/apps/**/*.cpp")
set(MAIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
set(DRIVER_SRC ${APP_SRCS} ${MAIN_SRC})
add_executable(driver ${DRIVER_SRC})
target_link_directories(driver PRIVATE ${DRIVER_INCLUDES})
target_link_libraries(driver PRIVATE shared)
message(STATUS "Driver includes at ${DRIVER_INCLUDES}")
message(STATUS "Driver source files are ${DRIVER_SRC}")

# test
add_executable(nsight_test ${CMAKE_CURRENT_SOURCE_DIR}/test/test.cpp)
target_link_libraries(nsight_test PRIVATE shared)