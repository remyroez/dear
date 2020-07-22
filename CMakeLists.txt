cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0076 NEW)

# CMake Module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/sdl2-cmake-modules)

# Project and Executable
project(dear C CXX)
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE)
target_compile_features(dear PRIVATE cxx_std_17)

# Dear ImGui
include(cmake/imgui.cmake)
target_link_libraries(${PROJECT_NAME} ${IMGUI_LIBRARIES})

# Emscripten options
if(EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set(TEMP_EXAMPLE_EMSCRIPTEN_SHELL ${IMGUI_EXAMPLE_DIR}/example_emscripten/shell_minimal.html)
    #set(TEMP_EXAMPLE_PRELOAD_FILE "MyImage01.jpg")
    set(TEMP_EXAMPLE_LINK_FLAGS "-s USE_SDL=2 -s WASM=1 -s FETCH=1")
    string(APPEND TEMP_EXAMPLE_LINK_FLAGS " -s ALLOW_MEMORY_GROWTH=1")
    string(APPEND TEMP_EXAMPLE_LINK_FLAGS " -s DISABLE_EXCEPTION_CATCHING=1 -s NO_EXIT_RUNTIME=0")
    string(APPEND TEMP_EXAMPLE_LINK_FLAGS " -s ASSERTIONS=1")
    #string(APPEND TEMP_EXAMPLE_LINK_FLAGS " --no-heap-copy --preload-file ${TEMP_EXAMPLE_PRELOAD_FILE}")
    #string(APPEND TEMP_EXAMPLE_LINK_FLAGS " --preload-file ${IMGUI_INCLUDE_DIR}/misc/fonts@/fonts")
    string(APPEND TEMP_EXAMPLE_LINK_FLAGS " --shell-file ${TEMP_EXAMPLE_EMSCRIPTEN_SHELL}")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS ${TEMP_EXAMPLE_LINK_FLAGS})
endif()

# Standalone application starter kit
include(cmake/imapp.cmake)
target_link_libraries(${PROJECT_NAME} ${IMAPP_LIBRARIES})

# cr
include(cmake/cr.cmake)
target_include_directories(${PROJECT_NAME} PRIVATE ${CR_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})

# sources
target_sources(${PROJECT_NAME} PRIVATE main.cpp)

#
set(DEAR_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# plugins
add_subdirectory(plugins)
