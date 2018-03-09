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

#include <GLDebugInitializer.h>
#include <Log.h>

#include <GLInclude.h>

namespace TierGine {

std::mutex GLDebugInitializer::critical;
bool GLDebugInitializer::initialized = false;
thread_local bool GLDebugInitializer::initializedThread = false;

namespace {

void MessageCallback( GLenum source,
                      GLenum type,
                      GLuint id,
                      GLenum severity,
                      GLsizei length,
                      const GLchar* message,
                      const void* userParam )
{
    if(type == GL_DEBUG_TYPE_ERROR) {
        Log::Error() << "GL ERRROR:" <<
                                  "type = " << type << ", severity = " << severity <<
                                  ", message = " << message << std::endl;
    } else{
        Log::Info() << "GL CALLBACK:" <<
                                  "type = " << type << ", severity = " << severity <<
                                  ", message = " << message << std::endl;
    }
}

}

bool GLDebugInitializer::Initialize()
{
    std::lock_guard<std::mutex> guard(critical);
    if( initialized ) {
        Log::Error() << "Trying to initialize already initialized GLDebug!" << std::endl;
        return false;
    }

    glEnable( GL_DEBUG_OUTPUT );
    glDebugMessageCallback( (GLDEBUGPROC) MessageCallback, 0 );

    initialized = true;
    return true;
}

bool GLDebugInitializer::InitializeThread()
{
    initializedThread = true;
    return true;
}

bool GLDebugInitializer::IsInitialized()
{
    std::lock_guard<std::mutex> guard(critical);
    return initialized;
}

}
