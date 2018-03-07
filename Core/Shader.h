#pragma once

#include <TierGine.CoreDefs.h>

#include <LoadableResource.h>

namespace TierGine {

interface IContext;

interface IShader : public ILoadableResource {
    enum Type {
        T_Vertex,
        T_TessellationControl,
        T_TessellationEvaluation,
        T_Geometry,
        T_Fragment,
        T_Compute
    };
    \
    IShader() {}
    virtual ~IShader() {}

    virtual bool IsValid() const = 0;
    virtual Type GetType() const = 0;
    virtual void LoadFromBuffer(const char* buffer) = 0;

    virtual IContext& GetContext() const = 0;

private:
    IShader(const IShader&) = delete;
    IShader& operator=(const IShader&) = delete;
};

}
