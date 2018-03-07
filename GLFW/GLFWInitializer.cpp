#include <GLFWInitializer.h>
#include <Log.h>

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
