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
#include <BasicModel.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace TierGine {

BasicModel::BasicModel(IMesh& mesh, glm::vec3 position, glm::vec3 rotation,
                       glm::vec3 scale,
                       TRenderingMode mode,
                       TPolygonRenderStyle style) :
    mat(glm::translate(glm::mat4(1.0f), position)),
    position(position),
    rotation(rotation),
    scale(scale),
    mode(mode),
    style(style),
    mesh(mesh),
    pipeline(nullptr)
{
    updateMat();
}

BasicModel::BasicModel(const BasicModel&& other):
    mat(other.mat),
    position(other.position),
    rotation(other.rotation),
    scale(other.scale),
    mode(other.mode),
    style(other.style),
    mesh(other.mesh),
    pipeline(other.pipeline)
{
}

void BasicModel::SetPosition(const glm::vec3& position)
{
    this->position = position;
    updateMat();
}

void BasicModel::SetRotation(const glm::vec3& rotation)
{
    this->rotation = rotation;
    updateMat();
}

void BasicModel::SetScale(const glm::vec3& scale)
{
    this->scale = scale;
    updateMat();
}

void BasicModel::SetRenderingMode(TRenderingMode mode, TPolygonRenderStyle style)
{
    this->mode = mode;
    this->style = style;
}

void BasicModel::Draw()
{
    mesh.Bind();
    mesh.GetRenderer().RenderWithMode(mode, style);
}

void BasicModel::LoadFromPath(std::string path)
{
    mesh.LoadFromPath(path);
}

void BasicModel::LoadFromTensors(Tensor vertices, Tensor normals)
{
    mesh.AddAtribute(0, vertices);
    mesh.AddAtribute(1, normals);
}

void BasicModel::LoadFromTensors(Tensor vertices, Tensor normals, Tensor uvTexture)
{
    mesh.AddAtribute(0, vertices);
    mesh.AddAtribute(1, normals);
    mesh.AddAtribute(2, uvTexture);
}

void BasicModel::updateMat()
{
    mat = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), scale);
}

}
