#pragma once

#include <assert.h>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <Context.h>
#include <WindowGLFW.h>
#include <GLShader.h>
#include <GLProgram.h>

namespace TierGine {

class GLFWContextGL : public IContext {
public:
    GLFWContextGL(WindowGLFW& window) : window(window) {}

    // IContext interface
    virtual TG_Status Activate() override;

    virtual IPipeline* CreatePipeline() override;
    virtual void DeletePipeline(IPipeline* pipeline) override;

    virtual IShader* CreateShader(IShader::Type shaderType) override;
    virtual void BindShader(const IShader* shader, IPipeline* pipeline) override;
    virtual void DeleteShader(IShader* shader) override;

private:
    WindowGLFW& window;
    std::unordered_map<const IShader*, std::unique_ptr<GLShader>> shaders;
    std::unordered_map<const IPipeline*, std::unique_ptr<GLProgram>> pipelines;
};

}
