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
#include <GLTexture.h>

#include <EngineException.h>

#include <SOIL2.h>

namespace TierGine {

GLTexture::GLTexture(IContext& context) :
    width(0),
    height(0),
    channels(0),
    context(context)
{
    glGenTextures(1, &id);
}

void GLTexture::LoadFromPath(std::string path)
{
    std::unique_ptr<unsigned char[]> imageBuffer(SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO));
    if(imageBuffer.get() == nullptr) {
        throw EngineException("Can not load texture from path " + path);
    }
    GLint internal = channels == 4 ? GL_RGBA8 : GL_RGB8;
    GLint format = channels == 4 ? GL_RGBA : GL_RGB;
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internal, width, height, 0, format, GL_UNSIGNED_BYTE, imageBuffer.get());
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture::Activate() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}


static GLint ParseTileMode(const GLTextureSampler::TileMode& mode) {
    switch (mode) {
    case GLTextureSampler::TM_None:
        return GL_CLAMP_TO_BORDER;
    case GLTextureSampler::TM_Repeat:
        return GL_REPEAT;
    default:
        assert(false);
        return 0;
    }
}

static GLint ParseInterploation(const GLTextureSampler::Interpolation& interpolation) {
    switch (interpolation) {
    case GLTextureSampler::I_Linear:
        return GL_LINEAR;
    case GLTextureSampler::I_Near:
        return GL_NEAREST;
    default:
        assert(false);
        return 0;
    }
}

GLTextureSampler::GLTextureSampler(IContext& context, std::string unifromName):
    uniformName(unifromName),
    currentTileMode(TM_Invalid),
    currentInterpolation(I_Invalid),
    context(context)
{
    glGenSamplers(1, &id);
    SetInterpolation(I_Linear);
    SetTileMode(TM_Repeat);
}

void GLTextureSampler::SetTileMode(const ITextureSampler::TileMode& mode) {
    if(currentTileMode != mode) {
        currentTileMode = mode;
        auto glMode = ParseTileMode(mode);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_T, glMode);
        glSamplerParameteri(id, GL_TEXTURE_WRAP_S, glMode);
    }
}

void GLTextureSampler::SetInterpolation(const Interpolation& interpolation) {
    if(currentInterpolation != interpolation) {
        currentInterpolation = interpolation;
        auto glInterpolation = ParseInterploation(interpolation);
        glSamplerParameteri(id, GL_TEXTURE_MAG_FILTER, glInterpolation);
        glSamplerParameteri(id, GL_TEXTURE_MIN_FILTER, glInterpolation);
    }
}

UniformVariable GLTextureSampler::GetUniform(IPipeline* pipeline)
{
    return pipeline->GetUniformVariable(uniformName);
}

void GLTextureSampler::BindAt(int textureLevelId)
{
    glBindSampler(textureLevelId, id);
}

}
