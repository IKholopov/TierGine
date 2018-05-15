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

#include <GLFWContextGL.h>
#include <GLMaterial.h>
#include <GLShader.h>

namespace TierGine {

IShader* GLFWContextGL::CreateShader(IShader::Type shaderType)
{
    GLShader* shader = new GLShader(shaderType, *this);
    shaders.insert({shader, nullptr}).first->second.reset(shader);
    return shader;
}

void GLFWContextGL::BindShader(const IShader* shader, IPipeline* pipeline)
{
    auto foundShader = shaders.find(shader);
    assert(foundShader != shaders.end());
    auto foundPipeline = pipelines.find(pipeline);
    assert(foundPipeline != pipelines.end());
    foundPipeline->second->BindShader(*foundShader->second);
}

TG_Status GLFWContextGL::Activate()
{
    glfwMakeContextCurrent(window.GetWindow());
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    return TG_Ok;
}

IPipeline* GLFWContextGL::CreatePipeline()
{
    GLProgram* program = new GLProgram(*this);
    pipelines.insert({program, nullptr}).first->second.reset(program);
    return program;
}

void GLFWContextGL::DeletePipeline(IPipeline* pipeline)
{
    auto storedPipeline = pipelines.find(pipeline);
    assert(storedPipeline != pipelines.end());
    pipelines.erase(storedPipeline);
}

void GLFWContextGL::DeleteShader(IShader* shader)
{
    auto storedShader = shaders.find(shader);
    assert(storedShader != shaders.end());
    shaders.erase(storedShader);
}

IMesh* GLFWContextGL::CreateMesh()
{
    GLMesh* mesh = new GLMesh(*this);
    meshes.insert({mesh, nullptr}).first->second.reset(mesh);
    return mesh;
}

void GLFWContextGL::DeleteMesh(const IMesh* mesh)
{
    auto storedMesh = meshes.find(mesh);
    assert(storedMesh != meshes.end());
    meshes.erase(storedMesh);
}

ITexture* GLFWContextGL::CreateTexture()
{
    GLTexture* texture = new GLTexture(*this);
    textures.insert({texture, nullptr}).first->second.reset(texture);
    return texture;
}

void GLFWContextGL::DeleteTexture(const ITexture* texture)
{
    auto storedTexture = textures.find(texture);
    assert(storedTexture != textures.end());
    textures.erase(storedTexture);
}

ITextureSampler* GLFWContextGL::CreateTextureSampler(const std::string& name)
{
    GLTextureSampler* sampler = new GLTextureSampler(*this, name);
    textureSamplers.insert({sampler, nullptr}).first->second.reset(sampler);
    return sampler;
}

void GLFWContextGL::DeleteTextureSampler(const ITextureSampler* sampler)
{
    auto storedSampler = textureSamplers.find(sampler);
    assert(storedSampler != textureSamplers.end());
    textureSamplers.erase(storedSampler);
}

IFramebuffer* GLFWContextGL::CreateFramebuffer(int width, int height)
{
    GLFramebuffer* buffer = new GLFramebuffer(*this, width, height);
    framebuffers.insert({buffer, nullptr}).first->second.reset(buffer);
    return buffer;
}

void GLFWContextGL::DeleteFramebuffer(const IFramebuffer* buffer)
{
    auto storedBuffer = framebuffers.find(buffer);
    assert(storedBuffer != framebuffers.end());
    framebuffers.erase(storedBuffer);
}

float GLFWContextGL::GetScreenDepthAt(int x, int y) const
{
    float value = -1.0f;
    glReadPixels(x, y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &value);
    return value;
}

std::unique_ptr<IMaterial> GLFWContextGL::CreateMaterial(ITextureSampler* sampler)
{
    return std::unique_ptr<IMaterial>(new GLMaterial(sampler));
}

}
