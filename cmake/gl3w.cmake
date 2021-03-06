
set(GL3W_PATH ${CMAKE_SOURCE_DIR}/thirdparty/imgui/examples/libs/gl3w)
set(GL3W_SOURCE_DIR ${GL3W_PATH}/GL)
set(GL3W_INCLUDE_DIR ${GL3W_PATH})

option(GL3W_BUILD_SHARED "Build shared for gl3w" OFF)

if(GL3W_BUILD_SHARED)
    add_library(gl3w SHARED)
else()
    add_library(gl3w STATIC)
endif()

target_sources(gl3w PRIVATE
    ${GL3W_SOURCE_DIR}/gl3w.c
)
target_include_directories(gl3w PUBLIC ${GL3W_INCLUDE_DIR})
target_link_libraries(gl3w ${CMAKE_DL_LIBS})

find_package(OpenGL REQUIRED)
target_link_libraries(gl3w OpenGL::GLX)

set(GL3W_LIBRARIES gl3w)
