cmake_minimum_required(VERSION 3.0)


file(GLOB Maze_SRC
    "*.h"
    "*.cpp"
)

set(shaders_path "./res/shaders/")
set(maze_path "./res/maze/")
set(texture_path "./res/textures/")

file(GLOB Maze_Shaders
    "${shaders_path}*.glsl"
)

file(GLOB Maze_Mazes
    "${maze_path}*.mz"
)

file(GLOB Maze_Textures
    "${texture_path}*.jpg"
)

foreach(shader ${Maze_Shaders})
    get_filename_component(shader_name ${shader} NAME)
    configure_file(${shader} ${CMAKE_CURRENT_BINARY_DIR}/${shaders_path}${shader_name})
endforeach(shader)

foreach(maze ${Maze_Mazes})
    get_filename_component(maze_name ${maze} NAME)
    configure_file(${maze} ${CMAKE_CURRENT_BINARY_DIR}/${maze_path}${maze_name})
endforeach(maze)

foreach(texture ${Maze_Textures})
    get_filename_component(texture_name ${texture} NAME)
    message(${CMAKE_CURRENT_BINARY_DIR}/${texture_path}${texture_name})
    file(COPY ${texture} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${texture_path})
endforeach(texture)

if(EXTERNAL_BUILD)
    set(TARGET_NAME 493Kholopov)
else()
    set(TARGET_NAME Maze)
    macro(MAKE_TASK TRGTNAME TASKNUM SOURCEFILES)
    endmacro(MAKE_TASK TRGTNAME SOURCEFILES)
endif()
add_definitions()

if(EXTERNAL_BUILD)
    MAKE_TASK(${TARGET_NAME} 1 ${Maze_SRC})
    set(TARGET_NAME "${TARGET_NAME}1")
else()
    add_executable(${TARGET_NAME} ${Maze_SRC})
endif()
target_include_directories(${TARGET_NAME} PRIVATE . ${TierGine.Core_DIR} ${TierGine.GLFW_DIR} ${TierGine.OpenGL_DIR} ${TierGine.EngineExt_DIR} ${TierGine.Formats_DIR} "${DEPENDENCIES_ROOT}/include/imgui")
target_link_libraries(${TARGET_NAME} Threads::Threads ${TIERGINECORELIB} ${TIERGINEGLFWLIB} ${TIERGINEOPENGLLIB} ${TIERGINEENGINEEXTLIB} )
