#pragma once

#include <Initializable.h>
#include <mutex>

namespace TierGine {

class GLEWInitializer : public IInitializable {
public:
    // IInitializable interface
    virtual bool Initialize() override;
    virtual bool InitializeThread() override;
    virtual void Deinitialize() override {}
    virtual void DeinitializeThread() override {}
    virtual bool IsInitialized() override;
    virtual bool IsInitializedOnThread() override { return initializedThread; }

private:
    static std::mutex critical;
    static bool initialized;
    thread_local static bool initializedThread;
};

}
