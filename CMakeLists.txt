cmake_minimum_required( VERSION 3.5 )

project( learn_glfw )

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory( deps/glfw )
add_subdirectory( deps/glew )

set(LEARN_GLFW_SRC main.cpp)

add_executable( learn_glfw ${LEARN_GLFW_SRC} )
target_link_libraries( learn_glfw ${OPENGL_LIBRARIES} glfw libglew_static )