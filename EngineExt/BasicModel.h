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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace TierGine {

class BasicModel : public IModel {
public:
    BasicModel(IMesh& mesh, glm::vec3 position = glm::vec3(0.0f),
               TRenderingMode mode=RM_FILL, TPolygonRenderStyle style=PRS_FRONT);
    BasicModel(const BasicModel&& other);

    // IWorldObject interface
    virtual const glm::mat4x4 GetPositionTransformation() const override
    { return mat; }
    virtual const glm::vec3& GetPosition() const override { return position; }
    virtual void SetPosition(const glm::vec3& position) override;

    // IDrawable interface
    virtual void SetRenderingMode(TRenderingMode mode, TPolygonRenderStyle style) override;
    virtual void Draw() override;

    // ILoadableResource interface
    virtual void LoadFromPath(std::string path) override;

    // IModel interface
    virtual void LoadFromTensors(Tensor vertices, Tensor normals) override;
    virtual void LoadFromTensors(Tensor vertices, Tensor normals, Tensor uvTexture) override;

private:
    glm::mat4 mat;
    glm::vec3 position;
    TRenderingMode mode;
    TPolygonRenderStyle style;
    IMesh& mesh;
};

}