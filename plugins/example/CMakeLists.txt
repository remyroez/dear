
# Project and Executable
project(example C CXX)
add_library(${PROJECT_NAME} SHARED)

# Dear ImGui
target_link_libraries(${PROJECT_NAME} ${IMGUI_LIBRARIES})

# cr
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty/cr)

# Example sources
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
