#pragma once

#include <Drawable.h>
#include <WorldObject.h>

namespace TierGine {

interface ISceneObject :
    public IDrawable,
    public IWorldObject
{
};

interface IScene {
    // Takes ownership
    virtual void Add(std::unique_ptr<ISceneObject>& object) = 0;
    virtual void Render() = 0;
    virtual void Update() = 0;
};

}
