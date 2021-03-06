
set(IMAPP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/imapp)
set(IMAPP_INCLUDE_DIR ${IMAPP_SOURCE_DIR})

option(IMAPP_BUILD_SHARED "Build shared for imapp" OFF)

# Standalone application starter kit
if(IMAPP_BUILD_SHARED)
  add_library(imapp SHARED)
  add_definitions(-fPIC)
else()
  add_library(imapp STATIC)
endif()
target_include_directories(imapp PUBLIC ${IMAPP_INCLUDE_DIR})

# System options
set(IMAPP_SYSTEM_EMSCRIPTEN OFF)
if(EMSCRIPTEN)
  set(IMAPP_SYSTEM_EMSCRIPTEN ON)
  target_compile_definitions(imapp PUBLIC IMAPP_SYSTEM_EMSCRIPTEN)
endif()

# Platform options
set(IMAPP_PLATFORM "SDL2" CACHE STRING "Platform Binding")
string(TOUPPER ${IMAPP_PLATFORM} IMAPP_PLATFORM)

set(IMAPP_PLATFORM_SDL2 OFF)
set(IMAPP_PLATFORM_GLFW OFF)

if(IMAPP_SYSTEM_EMSCRIPTEN)
  set(IMAPP_PLATFORM "SDL2")
endif()

if(IMAPP_PLATFORM STREQUAL "SDL2")
  set(IMAPP_PLATFORM_SDL2 ON)
  set(IMGUI_IMPL_PLATFORM_SDL2 ON)
elseif(IMAPP_PLATFORM STREQUAL "GLFW")
  set(IMAPP_PLATFORM_GLFW ON)
  set(IMGUI_IMPL_PLATFORM_GLFW ON)
endif()

# Platform Bindings
include(cmake/imgui_impl_platform.cmake)
target_link_libraries(imapp ${IMGUI_IMPL_PLATFORM_LIBRARIES})

# Renderer options
set(IMAPP_RENDERER "OpenGL3" CACHE STRING "Renderer Binding")
string(TOUPPER ${IMAPP_RENDERER} IMAPP_RENDERER)

set(IMAPP_RENDERER_OPENGL OFF)
set(IMAPP_RENDERER_OPENGL2 OFF)
set(IMAPP_RENDERER_OPENGL3 OFF)
set(IMAPP_RENDERER_VULKAN OFF)

if(IMAPP_SYSTEM_EMSCRIPTEN)
  set(IMAPP_RENDERER "OPENGL3")
endif()

if(IMAPP_RENDERER STREQUAL "OPENGL2")
  set(IMAPP_RENDERER_OPENGL ON)
  set(IMAPP_RENDERER_OPENGL2 ON)
  set(IMGUI_IMPL_RENDERER_OPENGL2 ON)
elseif(IMAPP_RENDERER STREQUAL "OPENGL3")
  set(IMAPP_RENDERER_OPENGL ON)
  set(IMAPP_RENDERER_OPENGL3 ON)
  set(IMGUI_IMPL_RENDERER_OPENGL3 ON)
elseif(IMAPP_RENDERER STREQUAL "VULKAN")
  set(IMAPP_RENDERER_VULKAN ON)
  set(IMGUI_IMPL_RENDERER_VULKAN ON)

  # Custom Vulkan Renderer Binding
  set(IMGUI_IMPL_RENDERER_VULKAN_CUSTOM_SOURCE ${IMAPP_SOURCE_DIR}/imapp_impl_vulkan.cpp)
endif()

# OpenGL Loader options
set(IMAPP_OPENGL_LOADER "GLEW" CACHE STRING "OpenGL Loader")
string(TOUPPER ${IMAPP_OPENGL_LOADER} IMAPP_OPENGL_LOADER)

set(IMAPP_OPENGL_LOADER_GLEW OFF)
set(IMAPP_OPENGL_LOADER_GL3W OFF)
set(IMAPP_OPENGL_LOADER_GLAD OFF)
set(IMAPP_OPENGL_LOADER_GLBINDING2 OFF)
set(IMAPP_OPENGL_LOADER_GLBINDING3 OFF)

if(NOT IMAPP_RENDERER_OPENGL3)
  # unnecessary
elseif(IMAPP_SYSTEM_EMSCRIPTEN)
  # unnecessary
elseif(IMAPP_OPENGL_LOADER STREQUAL "GLEW")
  set(IMAPP_OPENGL_LOADER_GLEW ON)
  set(IMGUI_IMPL_OPENGL_LOADER_GLEW ON)
elseif(IMAPP_OPENGL_LOADER STREQUAL "GL3W")
  set(IMAPP_OPENGL_LOADER_GL3W ON)
  set(IMGUI_IMPL_OPENGL_LOADER_GL3W ON)
elseif(IMAPP_OPENGL_LOADER STREQUAL "GLAD")
  set(IMAPP_OPENGL_LOADER_GLAD ON)
  set(IMGUI_IMPL_OPENGL_LOADER_GLAD ON)
elseif(IMAPP_OPENGL_LOADER STREQUAL "GLBINDING2")
  set(IMAPP_OPENGL_LOADER_GLBINDING2 ON)
  set(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2 ON)
elseif(IMAPP_OPENGL_LOADER STREQUAL "GLBINDING3")
  set(IMAPP_OPENGL_LOADER_GLBINDING3 ON)
  set(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3 ON)
endif()

# Renderer Bindings
include(cmake/imgui_impl_renderer.cmake)
target_link_libraries(imapp ${IMGUI_IMPL_RENDERER_LIBRARIES})

# Sources
target_sources(imapp PRIVATE
  ${IMAPP_SOURCE_DIR}/imapp.cpp
)

# Platform
if(IMAPP_PLATFORM_SDL2) # SDL2
  target_compile_definitions(imapp PUBLIC IMAPP_PLATFORM_SDL2)
  target_sources(imapp PRIVATE ${IMAPP_SOURCE_DIR}/imapp_platform_sdl.cpp)

elseif(IMAPP_PLATFORM_GLFW) # GLFW
  target_compile_definitions(imapp PUBLIC IMAPP_PLATFORM_GLFW)
  target_sources(imapp PRIVATE ${IMAPP_SOURCE_DIR}/imapp_platform_glfw.cpp)
endif()

# Renderer
if(IMAPP_RENDERER_OPENGL) # OpenGL
  target_compile_definitions(imapp PUBLIC IMAPP_RENDERER_OPENGL)
  target_sources(imapp PRIVATE ${IMAPP_SOURCE_DIR}/imapp_opengl_loader.cpp)

  if(IMAPP_RENDERER_OPENGL3) # OpenGL 3
    target_compile_definitions(imapp PUBLIC IMAPP_RENDERER_OPENGL3)
    target_sources(imapp PRIVATE ${IMAPP_SOURCE_DIR}/imapp_renderer_opengl3.cpp)

  elseif(IMAPP_RENDERER_OPENGL2) # OpenGL 2
    target_compile_definitions(imapp PUBLIC IMAPP_RENDERER_OPENGL2)
    target_sources(imapp PRIVATE ${IMAPP_SOURCE_DIR}/imapp_renderer_opengl2.cpp)
  endif()

elseif(IMAPP_RENDERER_VULKAN) # Vulkan
  target_compile_definitions(imapp PUBLIC IMAPP_RENDERER_VULKAN)
  target_sources(imapp PRIVATE ${IMAPP_SOURCE_DIR}/imapp_renderer_vulkan.cpp)
endif()

set(IMAPP_LIBRARIES imapp)
