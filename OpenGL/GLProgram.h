#pragma once

#include <Pipeline.h>
#include <GLShader.h>

namespace TierGine {

class GLProgram : public IPipeline {
public:
    GLProgram(IContext& context);
    ~GLProgram();

    // IPipeline interface
    virtual IContext& GetContext() const override { return context; }
    virtual void BindShader(const IShader* shader) override;
    virtual void Build() const override;
    virtual void Activate() const override;

    void BindShader(const GLShader& shader);

private:
    GLuint programId;
    IContext& context;
};

}
