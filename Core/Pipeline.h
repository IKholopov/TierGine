#pragma once

#include <Shader.h>

namespace TierGine {

interface IPipeline {
    IPipeline() {}
    virtual ~IPipeline() {}

    virtual IContext& GetContext() const = 0;
    virtual void BindShader(const IShader* shader) = 0;
    virtual void Build() const = 0;
    virtual void Activate() const = 0;

private:
    IPipeline(const IPipeline& other) = delete;
    IPipeline& operator=(const IPipeline& other) = delete;
};

}
