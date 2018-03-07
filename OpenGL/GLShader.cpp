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

#include <GLShader.h>

#include <assert.h>
#include <fstream>

#include <GLEWInitializer.h>
#include <Log.h>
#include <EngineException.h>

namespace TierGine {

namespace {
GLenum shaderTypeToGLEnum(IShader::Type type) {
    switch (type) {
    case IShader::T_Vertex:
        return GL_VERTEX_SHADER;
    case IShader::T_TessellationControl:
        return GL_TESS_CONTROL_SHADER;
    case IShader::T_TessellationEvaluation:
        return GL_TESS_EVALUATION_SHADER;
    case IShader::T_Geometry:
        return GL_GEOMETRY_SHADER;
    case IShader::T_Fragment:
        return GL_FRAGMENT_SHADER;
    case IShader::T_Compute:
        return GL_COMPUTE_SHADER;
    default:
        assert(false);
        return 0;
    }
}

}

GLShader::GLShader(Type type, IContext& context) :
    id(glCreateShader(shaderTypeToGLEnum(type))),
    type(type),
    context(context)
{
    GLEWInitializer glew;
    assert(glew.IsInitialized());
    if( id == 0 ){
        throw EngineException("Shader cannot be created");
    }
}

GLShader::~GLShader()
{
    glDeleteShader(id);
}

bool GLShader::IsValid() const
{
    // Not implemented
    assert(false);
}

IShader::Type GLShader::GetType() const
{
    return type;
}

void GLShader::LoadFromBuffer(const char* buffer)
{
    glShaderSource(id, 1, &buffer, NULL);
    glCompileShader(id);
    int status = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);
    if(status == GL_FALSE) {
        throw EngineException("Shader compilation failed");
    }
}

void GLShader::LoadFromPath(std::string path)
{
    try {
        std::ifstream file(path);
        if(!file.is_open()) {
            throw std::ifstream::failure("Failed to open file");
        }
        std::string str;

        file.seekg(0, std::ios::end);
        str.reserve(file.tellg());
        file.seekg(0, std::ios::beg);

        str.assign((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());
        LoadFromBuffer(str.c_str());
    } catch (std::ifstream::failure& e) {
        Log::Error() << "Failed to load file " << path << std::endl;
        throw;
     }
}

}
