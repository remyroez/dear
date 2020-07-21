
# Project and Library
project(example C CXX)
add_library(${PROJECT_NAME} SHARED)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

# Dear ImGui
target_link_libraries(${PROJECT_NAME} ${IMGUI_LIBRARIES})

# cr
target_include_directories(${PROJECT_NAME} PRIVATE ${CR_INCLUDE_DIR})

# sources
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
