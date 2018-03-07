#pragma once
// Simple proxy for creation of pipeline with vertex and fragment shader

#include <Pipeline.h>
#include <Context.h>


namespace TierGine {

class SimplePipeline : public IPipeline {
public:
    SimplePipeline(IContext& context, std::string vertexShaderPath,
                   std::string fragmentShaderPath);

    // IPipeline interface
    virtual IContext& GetContext() const override { return pipeline->GetContext(); }
    virtual void BindShader(const IShader* shader) override { GetContext().BindShader(shader, pipeline); }
    virtual void Build() const override { pipeline->Build(); }
    virtual void Activate() const override { pipeline->Activate(); }

private:
    IPipeline* pipeline;
};

}
