#pragma once

#include <GLEWInitializer.h>
#include <GLFWInitializer.h>
#include <GLFWContextGL.h>
#include <WindowGLFW.h>
#include <BaseApp.h>

namespace TierGine {

class GLBaseApp : public BaseApp {
public:
    GLBaseApp(const WindowGLFW::Config& config) :
        window(config, new GLFWContextGL(window))
    {
        Initializers().push_back(std::make_unique<GLFWInitializer>());
        Initializers().push_back(window.GetContextInitializer());
        Initializers().push_back(std::make_unique<GLEWInitializer>());
    }

protected:
    virtual TG_Status MainLoop() override { window.Update(); return TG_Ok; }
    virtual bool ShouldTerminate() const override { return window.ToClose(); }

    IContext* GetContext() const { return window.GetContext(); }

private:
    TierGine::WindowGLFW window;
};


}
