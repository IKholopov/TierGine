#pragma once

#include <TierGine.CoreDefs.h>

namespace TierGine {

interface IInitializable {
    virtual ~IInitializable() {}

    // Returns true on success, false on failure
    virtual bool Initialize() = 0;
    // Initialize ON NON-MAIN thread
    virtual bool InitializeThread() = 0;
    // Deinitialize on main thread
    virtual void Deinitialize() = 0;
    // Deinitialize ON NON-MAIN threa
    virtual void DeinitializeThread() = 0;

    // Check if initialized
    virtual bool IsInitialized() = 0;
    virtual bool IsInitializedOnThread() = 0;
};

}
