#pragma once

#include <TierGine.CoreDefs.h>

namespace TierGine {

enum TRenderingMode {
    RM_FILL,
    RM_WIRE,
    RM_POINTS
};

interface IDrawable {
    virtual void SetRenderingMode(TRenderingMode mode) = 0;
    virtual void Draw() = 0;
};

}
