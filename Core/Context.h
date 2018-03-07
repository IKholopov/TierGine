#pragma once
#include <Shader.h>
#include <Pipeline.h>

namespace TierGine {

interface IContext {
    virtual ~IContext() {}

    virtual IPipeline* CreatePipeline() = 0;
    virtual void DeletePipeline(IPipeline* pipeline) = 0;

    virtual IShader* CreateShader(IShader::Type shaderType) = 0;
    virtual void BindShader(const IShader* shader, IPipeline* pipeline) = 0;
    virtual void DeleteShader(IShader* shader) = 0;
    virtual TG_Status Activate() = 0;
};

}
