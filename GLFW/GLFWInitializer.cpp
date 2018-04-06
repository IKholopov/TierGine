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

#include <GLFWInitializer.h>
#include <Log.h>
#include <Time.h>

#include <GLFW/glfw3.h>

namespace TierGine {

bool GLFWInitializer::initialized = false;
thread_local bool GLFWInitializer::initializedThread = false;
std::mutex GLFWInitializer::critical;

namespace {

void glfwErrorCallback(int errorCode, const char* message)
{
    Log::Error() << "GLFW error occured, code: " << std::to_string(errorCode)<< L":" << std::endl;
    Log::Error() << message << std::endl;
}

class GLFWTimeProvider : public ITimeProvider {
public:
    // ITimeProvider interface
    virtual double GetTime() override { return glfwGetTime(); }
};

}

bool GLFWInitializer::Initialize()
{
    std::lock_guard<std::mutex> guard(critical);
    if( initialized ) {
        Log::Error() << "Trying initialize already initialized GLFW!" << std::endl;
        return false;
    }
    if( !InitializeThread() || !glfwInit() ) {
        Log::Error() << "GLFW initialization failed!" << std::endl;
        return false;
    }
    Time::TimeProvider.reset(new GLFWTimeProvider());
    initialized = true;
    int major, minor, rev = 0;
    glfwGetVersion(&major, &minor, &rev);
    Log::Info() << "Using GLFW v." << major << "." << minor << "-" << rev << std::endl;
    return true;
}

bool GLFWInitializer::InitializeThread()
{
    if( initializedThread ) {
        Log::Error() << "Trying to initialize GLFW on thread again!" << std::endl;
        return false;
    }
    glfwSetErrorCallback(&glfwErrorCallback);
    initializedThread = true;
    return true;
}

void GLFWInitializer::Deinitialize()
{
    std::lock_guard<std::mutex> guard(critical);
    assert(initialized);
    glfwTerminate();
}

}
