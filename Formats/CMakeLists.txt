cmake_minimum_required(VERSION 3.0)


file(GLOB Formats_SRC
    "*.h"
    "*.cpp"
)

set(TARGET_NAME TierGine.Formats)
set(TIERGINEFORMATSLIB ${TARGET_NAME} CACHE STRING "Formats lib name")
set(TierGine.Formats_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Formats include directories")
add_library(${TARGET_NAME} ${Formats_SRC})
target_include_directories(${TARGET_NAME} PRIVATE . ${TierGine.Core_DIR})
target_link_libraries(${TARGET_NAME} ${TIERGINECORELIB})
