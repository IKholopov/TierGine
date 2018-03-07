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

#include <GLProgram.h>

#include <Context.h>
#include <EngineException.h>

namespace TierGine {

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

void GLProgram::Build() const
{
    glLinkProgram(programId);
    int status = 0;
    glGetProgramiv(programId, GL_LINK_STATUS, &status);
    if(status != GL_TRUE) {
        throw EngineException("Failed to compile program");
    }
}

void GLProgram::Activate() const
{
    glUseProgram(programId);
}

void GLProgram::BindShader(const GLShader& shader)
{
    glAttachShader(programId, shader.GetShaderId());
}

}


