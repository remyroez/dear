
# Project and Library
project(example C CXX)
add_library(${PROJECT_NAME} SHARED)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

# Dear ImGui
target_link_libraries(${PROJECT_NAME} ${IMGUI_LIBRARIES})

# cr
target_include_directories(${PROJECT_NAME} PRIVATE ${CR_INCLUDE_DIR})

# dear
target_include_directories(${PROJECT_NAME} PRIVATE ${DEAR_INCLUDE_DIR})

# sources
target_sources(${PROJECT_NAME} PRIVATE main.cpp)
