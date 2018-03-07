/*
   Copyright 2018 Igor Kholopv, All rights reserved.

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

#include <WindowGLFW.h>

#include <Log.h>
#include <GLFWInitializer.h>

namespace TierGine {

WindowGLFW::WindowGLFW(const WindowGLFW::Config& config, IContext* context) :
    config(config),
    context(context)
{
}

std::unique_ptr<WindowGLFW::Initializer> WindowGLFW::GetContextInitializer()
{
    return std::make_unique<WindowGLFW::Initializer>(*this, true);
}

std::unique_ptr<WindowGLFW::Initializer> WindowGLFW::GetNoContextInitializer()
{
    return std::make_unique<WindowGLFW::Initializer>(*this, false);
}

void WindowGLFW::Update() const
{
    glfwPollEvents();
    glfwSwapBuffers(window);
}

TG_Status WindowGLFW::Create(bool useContext)
{
    GLFWInitializer initializer;
    assert(initializer.IsInitialized());
    if(!useContext) {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }
    GLFWmonitor* monitor = config.Fullscreen ? glfwGetPrimaryMonitor() : NULL;
    window = glfwCreateWindow(config.Width, config.Height, config.Title.c_str(),
                              monitor, NULL);
    if( !window ) {
        Log::Error() << "Failed to create requested window!" << std::endl;
        return TG_Critical;
    }
    if(useContext) {
        assert(context != nullptr);
        return context->Activate();
    }
    return TG_Ok;
}

}
