#include <SimplePipeline.h>

#include <assert.h>

namespace TierGine {

SimplePipeline::SimplePipeline(IContext& context, std::string vertexShaderPath,
                               std::string fragmentShaderPath) :
    pipeline(context.CreatePipeline())
{
    assert(pipeline != 0);
    auto vertexShader = GetContext().CreateShader(IShader::T_Vertex);
    vertexShader->LoadFromPath(vertexShaderPath);
    auto fragmentShader = GetContext().CreateShader(IShader::T_Fragment);
    fragmentShader->LoadFromPath(fragmentShaderPath);
    pipeline->BindShader(vertexShader);
    pipeline->BindShader(fragmentShader);
    pipeline->Build();
}

}
