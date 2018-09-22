cmake_minimum_required(VERSION 3.0)

project(TierGine)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${DEPENDENCIES_ROOT} "${DEPENDENCIES_ROOT}/cmake/soil" )
set(imgui_DIR "${DEPENDENCIES_ROOT}/lib/cmake/imgui")
set(SOIL_DIR "${DEPENDENCIES_ROOT}/lib/cmake/soil")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/" )
set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${DEPENDENCIES_ROOT}/include/imgui ${DEPENDENCIES_ROOT}/include")
set (CMAKE_CXX_STANDARD 17)

message(${DEPENDENCIES_ROOT})

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -Wno-sign-compare")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif()

find_package(glew)
find_package(glfw3)
find_package(glm)
find_package(assimp)
find_package(soil)
find_package(imgui)

find_package(Threads REQUIRED)

add_subdirectory(Core)

add_subdirectory(GLFW)
add_subdirectory(EngineExt)
add_subdirectory(Formats)
add_subdirectory(OpenGL)
add_subdirectory(sampleApp)
add_subdirectory(Maze)
