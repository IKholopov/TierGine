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
#include <Log.h>
#include <cmath>
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

glm::vec3 MazePortal::GetDirection() const {
    auto direction = entry->GetDirectionFrom(wall, forward);
    return glm::vec3(cos(direction[1]), 0.0f, sin(direction[1]));
}

glm::mat4x4 MazePortal::GetViewMatrix(bool direct) const
{
    assert(entry != nullptr);
    glm::vec2 direction = entry->GetDirectionFrom(wall, forward);
    glm::vec3 rotation = glm::vec3(direction[0], direct ? direction[1] : M_PI + direction[1], 0.0f);
    return glm::translate(glm::mat4(1.0f), entry->GetPositionFrom(wall, forward)) *
            glm::toMat4(glm::quat(rotation));
}

glm::vec4 MazePortal::GetPlane(float distance, const glm::mat4x4& camView) const
{
    assert(entry != nullptr);
    glm::vec2 direction = entry->GetDirectionFrom(wall, forward);
    glm::vec4 rotation = camView * glm::vec4(cos(direction[1]), 0.0f,  -sin(direction[1]), 0.0f);
    rotation[3] = -std::abs(distance);
    /*
    TG::Log::Info() << std::to_string(direction[0]) <<
                std::to_string(direction[1]) << std::endl;


    TG::Log::Info() << std::to_string(rotation[0]) << " " <<
                std::to_string(rotation[1]) << " " <<
                std::to_string(rotation[2]) << " " <<
                std::to_string(rotation[3]) << std::endl;
    */
    return rotation;
}

void MazePortal::GetUVs(const TierGine::CameraData& cam)
{
    auto geometry = GetGeometry();

/*   glm::vec2 mostLeftTop = glm::vec2(100, 100);
    glm::vec2 mostRightTop = glm::vec2(-100, 100);
    glm::vec2 mostLeftBottom = glm::vec2(100, -100);
    glm::vec2 mostRightBottom = glm::vec2(-100, -100);
*/
    for( int i = 0; i < geometry.GetSize(); ++i) {
        auto inScreen = glm::vec4(cam.Projection * cam.View * glm::vec4(geometry.GetPointer()[3 * i], geometry.GetPointer()[3 * i + 1], geometry.GetPointer()[3 * i + 2], 1.0f));
        /*
        TG::Log::Info() << std::to_string(1920*(inScreen[0]/inScreen[3] + 1)/2) << " " <<
                    std::to_string(1080*(inScreen[1]/inScreen[3] + 1)/2) << " " << std::endl;
                    */

/*
        int x = inScreen[0];
        int y = inScreen[1];
        if(x < mostLeftTop.x &&  y < mostLeftTop.y) {
            mostLeftTop = glm::vec2(x, y);
        }
        if(x > mostRightTop.x &&  y < mostRightTop.y) {
            mostRightTop = glm::vec2(x, y);
        }
        if(x < mostLeftBottom.x &&  y > mostLeftBottom.y) {
            mostLeftBottom = glm::vec2(x, y);
        }
        if(x > mostRightBottom.x &&  y > mostRightBottom.y) {
            mostRightBottom = glm::vec2(x, y);
        }
    }
    left = glm::vec4(mostLeftTop, mostLeftBottom);
    right = glm::vec4(mostRightTop, mostRightBottom);
    */
    }
    //TG::Log::Info() << "-----------" << std::endl;
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
