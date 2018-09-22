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

#include <Model.h>
#include <Material.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TierGine {

class BasicModel : public IModel {
public:
    BasicModel(IMesh& mesh, glm::vec3 position = glm::vec3(0.0f),
               glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f),
               TRenderingMode mode=RM_FILL, TPolygonRenderStyle style=PRS_FRONT);
    BasicModel(const BasicModel&& other);

    // IWorldObject interface
    virtual const glm::mat4x4 GetPositionTransformation() const override
    { return mat; }
    virtual const glm::vec3 GetPosition() const override { return position; }
    virtual void SetPosition(const glm::vec3& position) override;
    virtual const glm::vec3 GetRotation() const override { return rotation; }
    virtual void SetRotation(const glm::vec3& rotation) override;
    virtual const glm::vec3 GetScale() const override { return scale; }
    virtual void SetScale(const glm::vec3& scale) override;

    // IDrawable interface
    virtual void SetRenderingMode(TRenderingMode mode, TPolygonRenderStyle style) override;
    virtual void Draw() override;

    // ILoadableResource interface
    virtual void LoadFromPath(std::string path) override;

    // IModel interface
    virtual void LoadFromTensors(Tensor vertices, Tensor normals) override;
    virtual void LoadFromTensors(Tensor vertices, Tensor normals, Tensor uvTexture) override;
    virtual void AddTangentTensors(Tensor tangents, Tensor bitangent) override;
    virtual IPipeline* GetPipeline() const override { return pipeline; }
    virtual void SetPipeline(IPipeline* pipeline, UniformVariable textureVar=UniformVariable{}, UniformVariable normalMapVar=UniformVariable{},
                             UniformVariable materialVar=UniformVariable{}) override;

    void SetMaterial(IMaterial* material) { this->material = material; }
    const Tensor GetGeometry() const { return mesh.GetAtribute( 0 ); }
    void UpdateUVs(const Tensor& uv) { mesh.AddAtribute(2, uv); }

private:
    glm::mat4 mat;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    TRenderingMode mode;
    TPolygonRenderStyle style;
    IMesh& mesh;
    IPipeline* pipeline;
    UniformVariable textureValue;
    UniformVariable normalMapValue;
    UniformVariable materialVariable;
    IMaterial* material;

    void updateMat();
};

}
