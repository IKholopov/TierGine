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

#include <GLProgram.h>

#include <Context.h>
#include <EngineException.h>

namespace TierGine {

namespace {

UniformVariable::TUniformType GetUniformType(GLenum type) {
    switch (type) {
    case GL_FLOAT:
        return UniformVariable::UT_FLOAT;
        break;
    case GL_FLOAT_VEC2:
        return UniformVariable::UT_VEC_2;
        break;
    case GL_FLOAT_VEC3:
        return UniformVariable::UT_VEC_3;
        break;
    case GL_FLOAT_VEC4:
        return UniformVariable::UT_VEC_4;
        break;
    case GL_FLOAT_MAT2:
        return UniformVariable::UT_MAT_2;
        break;
    case GL_FLOAT_MAT3:
        return UniformVariable::UT_MAT_3;
        break;
    case GL_FLOAT_MAT4:
        return UniformVariable::UT_MAT_4;
        break;
    default:
        assert(false);
        break;
    }
}

}

GLProgram::GLProgram(IContext& context) :
    programId(glCreateProgram()),
    context(context)
{
}

GLProgram::~GLProgram()
{
    glDeleteProgram(programId);
}

void GLProgram::BindShader(const IShader* shader)
{
    GetContext().BindShader(shader, this);
}

void GLProgram::Build()
{
    glLinkProgram(programId);
    int status = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        throw EngineException("Failed to compile program");
    }
    GLint count, size;
    GLsizei length;
    GLenum type;
    const GLsizei bufSize = 64; // maximum name length
    GLchar name[bufSize];
    glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &count);
    for (GLint i = 0; i < count; i++)
    {
        glGetActiveUniform(programId, (GLuint)i, bufSize, &length, &size, &type, name);
        UniformVariable::TUniformType uniformType = GetUniformType(type);
        std::string strName(static_cast<char*>(name));
        auto var = UniformVariable(*this, uniformType, strName);
        uniforms.insert({strName, std::move(var)});
    }
}

void GLProgram::Activate() const
{
    glUseProgram(programId);
}

UniformVariable GLProgram::GetUniformVariable(std::string name) const
{
    auto variable = uniforms.find(name);
    if(variable == uniforms.end()) {
        throw EngineException("Trying to assign non-existing uniform - " + name);
    }
    return variable->second;
}

void GLProgram::SetUniformVariable(std::string name, Tensor value) const
{
    auto variable = uniforms.find(name);
    if(variable == uniforms.end()) {
        throw EngineException("Trying to assign non-existing uniform - " + name);
    }
    GLint uniformLoc = glGetUniformLocation(programId, name.c_str());
    setUniform(uniformLoc, value);
}

void GLProgram::BindShader(const GLShader& shader)
{
    glAttachShader(programId, shader.GetShaderId());
}

void GLProgram::setUniform(GLint uniformLoc, Tensor value) const
{
    switch (value.GetSize()) {
    case 4:
        assert(value.GetChannels() == 4);
        glProgramUniformMatrix4fv(programId, uniformLoc, 1, GL_FALSE,
                                  static_cast<const GLfloat*>(value.GetRawPointer()));
        break;
    case 3:
        assert(value.GetChannels() == 3);
        glProgramUniformMatrix3fv(programId, uniformLoc, 1, GL_FALSE,
                                  static_cast<const GLfloat*>(value.GetRawPointer()));
        break;
    case 2:
        assert(value.GetChannels() == 2);
        glProgramUniformMatrix2fv(programId, uniformLoc, 1, GL_FALSE,
                                  static_cast<const GLfloat*>(value.GetRawPointer()));
        break;
    case 1:
        switch (value.GetChannels()) {
        case 4:
            glProgramUniform4fv(programId, uniformLoc, 1,
                                static_cast<const GLfloat*>(value.GetRawPointer()));
            break;
        case 3:
            glProgramUniform3fv(programId, uniformLoc, 1,
                                static_cast<const GLfloat*>(value.GetRawPointer()));
            break;
        case 2:
            glProgramUniform2fv(programId, uniformLoc, 1,
                                static_cast<const GLfloat*>(value.GetRawPointer()));
            break;
        case 1:
            glProgramUniform1f(programId, uniformLoc,
                               *static_cast<const GLfloat*>(value.GetRawPointer()));
            break;
        default:
            break;
        }
    default:
        break;
    }
}

}


