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
#include <GLMesh.h>
#include <EngineException.h>
#include <Formats.h>
#include <Log.h>

namespace TierGine {

namespace {

GLenum RenderingMode(TRenderingMode mode) {
    switch (mode) {
    case RM_FILL:
        return GL_FILL;
    case RM_POINTS:
        return GL_POINT;
    case RM_WIRE:
        return GL_LINE;
    default:
        assert(false);
        break;
    }
}

GLenum PolygonStyle(TPolygonRenderStyle mode) {
    switch (mode) {
    case PRS_FRONT:
        return GL_FRONT;
    case PRS_BACK:
        return GL_BACK;
    case PRS_BOTH:
        return GL_FRONT_AND_BACK;
    default:
        assert(false);
        break;
    }
}

GLenum Type(Tensor::Type type) {
    switch (type) {
    case Tensor::T_BOOL:
        return GL_BOOL;
    case Tensor::T_BYTE:
        return GL_BYTE;
    case Tensor::T_UBYTE:
        return GL_UNSIGNED_BYTE;
    case Tensor::T_SHORT:
        return GL_SHORT;
    case Tensor::T_USHORT:
        return GL_UNSIGNED_SHORT;
    case Tensor::T_INT:
        return GL_INT;
    case Tensor::T_UINT:
        return GL_UNSIGNED_INT;
    case Tensor::T_FLOAT:
        return GL_FLOAT;
    case Tensor::T_DOUBLE:
        return GL_DOUBLE;
    default:
        assert(false);
        break;
    }
}

}

GLMesh::GLMesh(IContext& context) :
    renderer(0),
    context(context),
    size(0),
    vao(0)
{
    glGenVertexArrays(1, &vao);
}

GLMesh::~GLMesh()
{
    glDeleteVertexArrays(1, &vao);
}

void GLMesh::LoadFromPath(std::string path)
{
    std::unique_ptr<IMeshLoader> loader = Formats::GetLoaderFor(path);
    loader->LoadInto(*this);
}

void GLMesh::AddAtribute(int id, const Tensor& value)
{
    if(attributes.find(id) == attributes.end()) {
        attributes.insert({id, std::make_unique<Buffer>(value)});
        if(id == 0) {
            this->size = value.GetSize();
            renderer.SetSize(value.GetSize());
        }
    } else {
        assert(this->Size() == value.GetSize());
        attributes.find(id)->second->SetData(value);
        return;
    }
    auto& buffer = attributes.find(id)->second;
    glBindVertexArray(vao);
    buffer->Bind();
    glEnableVertexAttribArray(id);
    glVertexAttribPointer(id, value.GetChannels(), Type(value.GetType()), GL_FALSE, 0, 0);
    buffer->Unbind();
    glBindVertexArray(0);
}

void GLMesh::Bind() const
{
    assert(attributes.find(0) != attributes.end());
    glBindVertexArray(vao);
}

void GLMesh::Renderer::RenderWithMode(TRenderingMode mode, TPolygonRenderStyle style) const
{
    if(size <= 0) {
        throw new EngineException("Trying to render empty GLMesh");
    }
    glPolygonMode(GL_FRONT_AND_BACK, RenderingMode(mode));
    if(style == PRS_BOTH) {
        glDisable(GL_CULL_FACE);
    } else {
        glEnable(GL_CULL_FACE);
        glCullFace(PolygonStyle(style));
    }
    glDrawArrays(GL_TRIANGLES, 0, size);
}

GLMesh::Buffer::Buffer(const Tensor& data) : data(data)
{
    glGenBuffers(1, &vbo);
    Bind();
    glBufferData(GL_ARRAY_BUFFER, data.GetSize() * data.GetChannels() * sizeof(float), data.GetRawPointer(), GL_DYNAMIC_DRAW);

    Unbind();
}

GLMesh::Buffer::~Buffer()
{
    glDeleteBuffers(1, &vbo);
}

void GLMesh::Buffer::SetData(const Tensor& data)
{
    Bind();
    this->data = data;
    glBufferSubData(GL_ARRAY_BUFFER, 0, data.GetSize() * data.GetChannels() * sizeof(float), data.GetRawPointer());
    glGetError();
    Unbind();
}

void GLMesh::Buffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void GLMesh::Buffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



}
