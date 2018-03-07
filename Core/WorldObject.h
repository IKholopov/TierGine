// Object with coordinates
#pragma once

#include <TierGine.CoreDefs.h>
#include <glm/glm.hpp>

namespace TierGine {

interface IWorldObject {
    virtual const glm::mat4x4& GetPositionTransformation() = 0;
    virtual void SetPositionTransformation() = 0;
};

}
