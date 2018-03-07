cmake_minimum_required(VERSION 3.0)


file(GLOB Core_SRC
    "*.h"
    "*.cpp"
)
set(TARGET_NAME TierGine.Core)
set(TIERGINECORELIB ${TARGET_NAME} CACHE STRING "Core lib name")
set(TierGine.Core_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Core include directories")
add_library(${TARGET_NAME} ${Core_SRC})
target_include_directories(${TARGET_NAME} PRIVATE .)
target_link_libraries(${TARGET_NAME} GLEW glfw GL)
