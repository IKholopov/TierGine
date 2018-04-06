cmake_minimum_required(VERSION 3.0)


file(GLOB OpenGL_SRC
    "*.h"
    "*.cpp"
)
set(TARGET_NAME TierGine.OpenGL)
set(TIERGINEOPENGLLIB ${TARGET_NAME} CACHE STRING "OpenGL lib name")
set(TierGine.OpenGL_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "OpenGL include directories")
add_library(${TARGET_NAME} ${OpenGL_SRC})
target_include_directories(${TARGET_NAME} PRIVATE . ${TierGine.Core_DIR} ${TierGine.GLFW_DIR} ${TierGine.Formats_DIR})
target_link_libraries(${TARGET_NAME} GLEW GL Threads::Threads ${TIERGINEFORMATSLIB})
