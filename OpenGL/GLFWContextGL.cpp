#include <GLFWContextGL.h>
#include <GLShader.h>

namespace TierGine {

IShader* GLFWContextGL::CreateShader(IShader::Type shaderType)
{
    GLShader* shader = new GLShader(shaderType, *this);
    shaders.insert({shader, nullptr}).first->second.reset(shader);
    return shader;
}

void GLFWContextGL::BindShader(const IShader* shader, IPipeline* pipeline)
{
    auto foundShader = shaders.find(shader);
    assert(foundShader != shaders.end());
    auto foundPipeline = pipelines.find(pipeline);
    assert(foundPipeline != pipelines.end());
    foundPipeline->second->BindShader(*foundShader->second);
}

TG_Status GLFWContextGL::Activate()
{
    glfwMakeContextCurrent(window.GetWindow());
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    return TG_Ok;
}

IPipeline* GLFWContextGL::CreatePipeline()
{
    GLProgram* program = new GLProgram(*this);
    pipelines.insert({program, nullptr}).first->second.reset(program);
    return program;
}

void GLFWContextGL::DeletePipeline(IPipeline* pipeline)
{
    auto storedPipeline = pipelines.find(pipeline);
    assert(storedPipeline != pipelines.end());
    pipelines.erase(storedPipeline);
}

void GLFWContextGL::DeleteShader(IShader* shader)
{
    auto storedShader = shaders.find(shader);
    assert(storedShader != shaders.end());
    shaders.erase(storedShader);
}

}
