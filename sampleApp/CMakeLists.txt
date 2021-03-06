cmake_minimum_required(VERSION 3.0)


file(GLOB sampleApp_SRC
    "*.h"
    "*.cpp"
)

set(shaders_path "./res/shaders/")

file(GLOB sampleApp_Shaders
    "${shaders_path}*.glsl"
)

foreach(shader ${sampleApp_Shaders})
    get_filename_component(shader_name ${shader} NAME)
    configure_file(${shader} ${CMAKE_CURRENT_BINARY_DIR}/${shaders_path}${shader_name})
endforeach(shader)

set(TARGET_NAME SampleApp)


add_executable(${TARGET_NAME} ${sampleApp_SRC})

target_include_directories(${TARGET_NAME} PRIVATE ${TierGine.Core_DIR} ${TierGine.GLFW_DIR} ${TierGine.OpenGL_DIR} ${TierGine.EngineExt_DIR})
target_link_libraries(${TARGET_NAME} Threads::Threads ${TIERGINECORELIB} ${TIERGINEGLFWLIB} ${TIERGINEOPENGLLIB} ${TIERGINEENGINEEXTLIB} )
