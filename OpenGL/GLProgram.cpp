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


