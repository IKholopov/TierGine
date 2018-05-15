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

#pragma once

#include <assert.h>
#include <vector>
#include <unordered_map>

#include <GL/glew.h>
#include <Context.h>
#include <Backend.h>
#include <WindowGLFW.h>
#include <GLShader.h>
#include <GLProgram.h>
#include <GLTexture.h>
#include <GLMesh.h>
#include <GLFramebuffer.h>

namespace TierGine {

class GLFWContextGL : public IContext, public IBackend {
public:
    GLFWContextGL(WindowGLFW& window) : window(window) {}

    // IContext interface
    virtual TG_Status Activate() override;

    virtual IPipeline* CreatePipeline() override;
    virtual void DeletePipeline(IPipeline* pipeline) override;

    virtual IShader* CreateShader(IShader::Type shaderType) override;
    virtual void BindShader(const IShader* shader, IPipeline* pipeline) override;
    virtual void DeleteShader(IShader* shader) override;

    virtual IMesh* CreateMesh() override;
    virtual void DeleteMesh(const IMesh* mesh) override;

    virtual ITexture* CreateTexture() override;
    virtual void DeleteTexture(const ITexture* texture) override;

    virtual ITextureSampler* CreateTextureSampler(const std::string& name) override;
    virtual void DeleteTextureSampler(const ITextureSampler* sampler) override;

    virtual IFramebuffer* CreateFramebuffer(int width, int height) override;
    virtual void DeleteFramebuffer(const IFramebuffer* buffer) override;

    virtual float GetScreenDepthAt(int x, int y) const override;

    // IBackend interface
    virtual std::unique_ptr<IMaterial> CreateMaterial(ITextureSampler* sampler) override;

private:
    WindowGLFW& window;
    std::unordered_map<const IShader*, std::unique_ptr<GLShader>> shaders;
    std::unordered_map<const IPipeline*, std::unique_ptr<GLProgram>> pipelines;
    std::unordered_map<const IMesh*, std::unique_ptr<GLMesh>> meshes;
    std::unordered_map<const ITexture*, std::unique_ptr<GLTexture>> textures;
    std::unordered_map<const ITextureSampler*, std::unique_ptr<GLTextureSampler>> textureSamplers;
    std::unordered_map<const IFramebuffer*, std::unique_ptr<GLFramebuffer>> framebuffers;
};

}
