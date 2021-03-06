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

#include <WindowGLFW.h>

#include <Log.h>
#include <GLFWInitializer.h>

namespace TierGine {

std::mutex WindowGLFW::windowCritical;
std::unordered_map<GLFWwindow*, WindowGLFW*> WindowGLFW::   windows;

WindowGLFW::WindowGLFW(const WindowGLFW::Config& config, IContext* context) :
    config(config),
    context(context)
{
    inputListeners.push_back(inputManager.AddKeyListener(GLFW_KEY_F11, [this](int action) {
        if(action == GLFW_PRESS) { this->Fullscreen(); }
    }, 0.5f));
    inputListeners.push_back(inputManager.AddKeyListener(GLFW_KEY_ESCAPE, [this](int action) {
        if(action == GLFW_PRESS) { this->Close(); }
    }));
}

WindowGLFW::~WindowGLFW()
{
    std::lock_guard<std::mutex> guard(windowCritical);
    windows.erase(windows.find(window));
}

std::unique_ptr<WindowGLFW::Initializer> WindowGLFW::GetContextInitializer()
{
    return std::make_unique<WindowGLFW::Initializer>(*this, true);
}

std::unique_ptr<WindowGLFW::Initializer> WindowGLFW::GetNoContextInitializer()
{
    return std::make_unique<WindowGLFW::Initializer>(*this, false);
}

void WindowGLFW::Resize(int width, int height)
{
    glfwSetWindowSize(window, width, height);
}

void WindowGLFW::Move(int x, int y)
{
    glfwSetWindowPos(window, x, y);
}

void WindowGLFW::Fullscreen()
{
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if(config.Fullscreen) {
        config.Fullscreen = false;
        glfwSetWindowMonitor(window, NULL, 0, 0, config.Width, config.Height, mode->refreshRate);
        return;
    }
    config.Fullscreen = true;
    glfwSetWindowMonitor(window, monitor, 0, 0, config.Width, config.Height, mode->refreshRate);
}

void WindowGLFW::Update()
{
    glfwSwapBuffers(window);
}

void WindowGLFW::RegularUpdate()
{
    auto& keySubs = inputManager.GetKeySubscribers();
    for(auto it = keySubs.begin(); it != keySubs.end(); ++it) {
        if(glfwGetKey(window, it->first) == GLFW_PRESS) {
            inputManager.OnKey(it->first, GLFW_PRESS);
        }
    }

    auto& mouseButtonSubs = inputManager.GetMouseButtonSubscribers();
    for(auto it = mouseButtonSubs.begin(); it != mouseButtonSubs.end(); ++it) {
        if(glfwGetMouseButton(window, it->first) == GLFW_PRESS) {
            inputManager.OnMouseButton(it->first);
        }
    }

    glfwPollEvents();
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
    glfwSetCursorPosCallback(window, &mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    windowCritical.lock();
    windows.insert({window, this});
    windowCritical.unlock();
    if(useContext) {
        assert(context != nullptr);
        return context->Activate();
    }
    return TG_Ok;
}

void WindowGLFW::Close()
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void WindowGLFW::mouseCallback(GLFWwindow* window, double x, double y)
{
    windows.at(window)->inputManager.OnMouse(x, y);
}

}
