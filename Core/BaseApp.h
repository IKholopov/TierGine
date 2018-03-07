#pragma once

#include <Initializable.h>

#include <vector>

#include <Log.h>
#include <Window.h>

namespace TierGine {

class BaseApp {
public:
    BaseApp();
    virtual ~BaseApp() {}

    virtual bool InitializeApp();
    virtual void DeinitializeApp();

    virtual TG_Status Main();

protected:
    virtual TG_Status MainLoop() = 0;
    virtual bool ShouldTerminate() const = 0;

    std::vector<std::unique_ptr<IInitializable>>& Initializers() { return initializers; }

private:
    std::vector<std::unique_ptr<IInitializable>> initializers;
    Log baseLog;
    Log errorLog;
};

}
