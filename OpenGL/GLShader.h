#pragma once

#include <Shader.h>
#include <GLInclude.h>

namespace TierGine {

class GLShader : public IShader {
public:
    GLShader(IShader::Type type, IContext& context);
    ~GLShader();

    // IShader interface
    virtual bool IsValid() const override;
    virtual Type GetType() const override;
    virtual void LoadFromBuffer(const char* buffer) override;
    virtual void LoadFromPath(std::string path) override;

    virtual IContext& GetContext() const override { return context; }

    GLuint GetShaderId() const { return id; }

private:
    GLuint id;
    IShader::Type type;
    IContext& context;
};

}
