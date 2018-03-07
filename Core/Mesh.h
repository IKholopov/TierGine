#pragma once

#include <Tensor.h>
#include <LoadableResource.h>

namespace TierGine {

interface IMesh : public ILoadableResource {
    virtual void AddAtribute(int id, Tensor value) = 0;
};

}
