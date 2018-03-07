#pragma once

#include <TierGine.CoreDefs.h>

interface ILoadableResource {
    virtual void LoadFromPath(std::string path) = 0;
};
