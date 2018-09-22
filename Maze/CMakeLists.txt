cmake_minimum_required(VERSION 3.0)


file(GLOB Maze_SRC
    "*.h"
    "*.cpp"
)

set(shaders_path "./res/shaders/")
set(texture_path "./res/textures/")

file(GLOB Maze_Shaders
    "${shaders_path}*.glsl"
)

file(GLOB Maze_Textures
    "${texture_path}*.jpg"
)

foreach(shader ${Maze_Shaders})
    get_filename_component(shader_name ${shader} NAME)
    configure_file(${shader} ${CMAKE_CURRENT_BINARY_DIR}/${shaders_path}${shader_name})
endforeach(shader)

foreach(texture ${Maze_Textures})
    get_filename_component(texture_name ${texture} NAME)
    message(${CMAKE_CURRENT_BINARY_DIR}/${texture_path}${texture_name})
    file(COPY ${texture} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${texture_path})
endforeach(texture)

set (CMAKE_CXX_STANDARD 17)

set(TARGET_NAME Maze)

add_executable(${TARGET_NAME} ${Maze_SRC})
target_include_directories(${TARGET_NAME} PRIVATE . ${TierGine.Core_DIR} ${TierGine.GLFW_DIR} ${TierGine.OpenGL_DIR} ${TierGine.EngineExt_DIR} ${TierGine.Formats_DIR})
target_link_libraries(${TARGET_NAME} Threads::Threads ${TIERGINECORELIB} ${TIERGINEGLFWLIB} ${TIERGINEOPENGLLIB} ${TIERGINEENGINEEXTLIB} )
