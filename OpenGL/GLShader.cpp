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
