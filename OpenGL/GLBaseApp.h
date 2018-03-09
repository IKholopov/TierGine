/*
   Copyright 2018 Igor Kholopov, All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
   ==============================================================================
*/

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
    virtual TG_Status MainLoop() override
    { window.Update(); glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);return TG_Ok; }
    virtual bool ShouldTerminate() const override { return window.ToClose(); }

    IContext* GetContext() const { return window.GetContext(); }
    InputProvider& GetInputProvider() { return window.GetInputProvider(); }

private:
    TierGine::WindowGLFW window;
};


}
