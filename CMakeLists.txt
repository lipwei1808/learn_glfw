cmake_minimum_required( VERSION 3.5 )

project( learn_glfw )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( deps/glfw )
add_subdirectory( deps/glew )

file(GLOB_RECURSE MY_HEADER_FILES src/*.h)
file(GLOB_RECURSE MY_SOURCE_FILES src/*.cpp)

add_executable( learn_glfw ${MY_SOURCE_FILES} ${MY_HEADER_FILES})
target_link_libraries( learn_glfw ${OPENGL_LIBRARIES} glfw libglew_static )