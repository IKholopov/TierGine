cmake_minimum_required(VERSION 3.0)


file(GLOB EngineExt_SRC
    "*.h"
    "*.cpp"
)
set(TARGET_NAME TierGine.EngineExt)
set(TIERGINEENGINEEXTLIB ${TARGET_NAME} CACHE STRING "EngineExt lib name")
set(TierGine.EngineExt_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "EngineExt include directories")
add_library(${TARGET_NAME} ${EngineExt_SRC})
target_include_directories(${TARGET_NAME} PRIVATE . ${TierGine.Core_DIR})
target_link_libraries(${TARGET_NAME} Threads::Threads ${TIERGINECORELIB})
