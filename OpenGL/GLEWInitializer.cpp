#include <GLEWInitializer.h>
#include <Log.h>

#include <GLInclude.h>

namespace TierGine {

std::mutex GLEWInitializer::critical;
bool GLEWInitializer::initialized = false;
thread_local bool GLEWInitializer::initializedThread = false;

bool GLEWInitializer::Initialize()
{
    std::lock_guard<std::mutex> guard(critical);
    if( initialized ) {
        Log::Error() << "Trying to initialize already initialized GLEW!" << std::endl;
        return false;
    }
    glewExperimental = GL_TRUE;
    if( !InitializeThread() || glewInit() ) {
        Log::Error() << "Failed to initialize glew" << std::endl;
        return false;
    }
    initialized = true;
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    Log::Info() << "Renderer: " << std::string(reinterpret_cast<const char*>(renderer)) << std::endl;
    Log::Info() << "Using OpenGL v." << std::string(reinterpret_cast<const char*>(version)) << std::endl;
    return true;
}

bool GLEWInitializer::InitializeThread()
{
    initializedThread = true;
    return true;
}

bool GLEWInitializer::IsInitialized()
{
    std::lock_guard<std::mutex> guard(critical);
    return initialized;
}

}
