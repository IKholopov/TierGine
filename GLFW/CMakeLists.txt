cmake_minimum_required(VERSION 3.0)


file(GLOB glfwSupport_SRC
    "*.h"
    "*.cpp"
)
set(TARGET_NAME TierGine.GLFW)
set(TIERGINEGLFWLIB ${TARGET_NAME} CACHE STRING "GLFW lib name")
set(TierGine.GLFW_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "GLFW include directories")
add_library(${TARGET_NAME} ${glfwSupport_SRC})
target_include_directories(${TARGET_NAME} PRIVATE . ${TierGine.Core_DIR})
target_link_libraries(${TARGET_NAME} glfw ${TIERGINECORELIB})
