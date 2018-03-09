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

#include <GLInclude.h>
#include <Context.h>
#include <Mesh.h>
#include <Drawable.h>

namespace TierGine {

class GLMesh : public IMesh {
public:
    class Renderer : public IRenderer {
    public:
        Renderer(int size) : size(size) {}

        virtual void RenderWithMode(TRenderingMode mode, TPolygonRenderStyle style) const override;

        void SetSize(int size) { this->size = size; }

    private:
        int size;
    };

    GLMesh(IContext& context);
    ~GLMesh();

    // ILoadableResource interface
    virtual void LoadFromPath(std::string path) override;

    // IMesh interface
    virtual int Size() const override { return size; }
    virtual const IRenderer& GetRenderer() const override { return renderer; }
    virtual const IContext& GetContext() const override { return context; }

    virtual void AddAtribute(int id, const Tensor& value) override;
    virtual void Bind() const override;

private:
    class Buffer {
    public:
        Buffer(const Tensor data);
        ~Buffer();

        void Bind();
        void Unbind();

    private:
        const Tensor data;
        GLuint vbo;

        Buffer(const Buffer& other) = delete;
        Buffer(const Buffer&& other) = delete;
    };

    Renderer renderer;
    std::unordered_map<int, std::unique_ptr<Buffer> > attributes;
    IContext& context;
    int size;
    GLuint vao;
};

}
