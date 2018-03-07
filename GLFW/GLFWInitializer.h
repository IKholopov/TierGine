#pragma once

#include <Initializable.h>
#include <mutex>
#include <assert.h>

namespace TierGine {

class GLFWInitializer : public IInitializable {
public:
    // IInitializable interface
    virtual bool Initialize() override;
    virtual bool InitializeThread() override;
    virtual void Deinitialize() override;
    virtual void DeinitializeThread() override { assert(initializedThread); }

    virtual bool IsInitialized() override { return initialized; }
    virtual bool IsInitializedOnThread() override { return initializedThread; }

private:
    static bool initialized;
    static std::mutex critical;
    thread_local static bool initializedThread;
};

}
