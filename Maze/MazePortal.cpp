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
#include <MazePortal.h>
#include <MazePoster.h>
#include <TBN.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

static const char* const ColorVariableName = "portalColor";

void MazePortal::SetMoveTo(const WalledEntry* entry, const unsigned char& wall, bool forward)
{
    opened = false;
    this->entry = entry;
    this->wall = wall;
    this->forward = forward;
}

void MazePortal::Draw()
{
    if(!opened && entry != nullptr) {
        applyMove();
    }
    GetPipeline()->SetUniformVariable(ColorVariableName, TG::CreateTensor(color));
    if(opened) {
        BasicModel::Draw();
    }
}

void MazePortal::SetPipeline(TierGine::IPipeline* pipeline,
                             TierGine::UniformVariable textureVar,
                             TierGine::UniformVariable normalMapVar,
                             TierGine::UniformVariable materialVar)
{
    assert(pipeline->HasUniformVariable(ColorVariableName));
    BasicModel::SetPipeline(pipeline, textureVar, normalMapVar, materialVar);
}

glm::mat4x4 MazePortal::GetViewMatrix(bool direct) const
{
    assert(entry != nullptr);
    glm::vec2 direction = entry->GetDirectionFrom(wall, forward);
    glm::vec3 rotation = glm::vec3(direction[0], direct ? direction[1] : M_PI + direction[1], 0.0f);
    return glm::translate(glm::mat4(1.0f), entry->GetPositionFrom(wall, forward)) *
            glm::toMat4(glm::quat(rotation));
}

glm::vec4 MazePortal::GetPlane() const
{
    assert(entry != nullptr);
    glm::vec2 direction = entry->GetDirectionFrom(wall, forward);
    glm::vec3 rotation = glm::vec3(direction[0], direction[1], 0.0f);

    return glm::vec4(glm::quat(rotation) * glm::vec3(0.0f, 0.0f, -1.0f), glm::length(entry->GetPositionFrom(wall, forward)));
}

void MazePortal::applyMove()
{
    WalledEntry wallEntry(*entry, wall);
    MazePoster poster(&wallEntry, forward);
    auto [v, norm, uv] = poster.GetMesh();
    auto [tan, bitan] = TG::TBN::CalculateTangentAndBitangent(v, uv);
    LoadFromTensors(v, norm, uv);
    AddTangentTensors(tan, bitan);
    opened = true;
}
