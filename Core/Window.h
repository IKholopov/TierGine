#pragma once
#include <TierGine.CoreDefs.h>

namespace TierGine {

interface IWindow {
    virtual ~IWindow() {}

    virtual void Resize(int width, int height) = 0;
    virtual void Move(int x, int y) = 0;
    virtual void Fullscreen() = 0;
    virtual void Update() const = 0;
    virtual bool ToClose() const = 0;
};

}
