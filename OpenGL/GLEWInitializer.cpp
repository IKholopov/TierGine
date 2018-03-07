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
