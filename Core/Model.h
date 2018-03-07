#pragma once

#include <Scene.h>
#include <Mesh.h>

namespace TierGine {

interface IModel :
    public IMesh,
    public ISceneObject,
    public ILoadableResource
{
    virtual void LoadFromTensors(Tensor vertices, Tensor normals) = 0;
    virtual void LoadFromTensors(Tensor vertices, Tensor normals, Tensor uvTexture) = 0;
};

}
