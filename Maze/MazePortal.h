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
#include <BasicModel.h>
#include <Camera.h>
#include <MazeGrid.h>

namespace TG = TierGine;

class MazePortal: public TG::BasicModel {
public:
    MazePortal(TG::IMesh& mesh,
               const glm::vec3& color=glm::vec3(0.2f, 0.3f, 0.9f)):
        BasicModel(mesh),
        color(color),
        entry(nullptr),
        wall(0),
        forward(false),
        opened(false)
    {}

    void SetMoveTo(const WalledEntry* entry, const Byte& wall, bool forward);

    // BasicModel
    virtual void Draw() override;
    virtual void SetPipeline(TG::IPipeline* pipeline, TG::UniformVariable textureVar=TG::UniformVariable{},
                             TG::UniformVariable normalMapVar=TG::UniformVariable{},
                             TG::UniformVariable materialVar=TG::UniformVariable{}) override;
    virtual const glm::vec3 GetPosition() const override { return entry->GetPositionFrom(wall, forward); }

    glm::vec3 GetDirection() const;

    glm::mat4x4 GetViewMatrix(bool direct) const;
    glm::vec4 GetPlane(float distance, const glm::mat4x4& camView) const;
    bool IsOpened() const { return opened; }

private:
    glm::vec3 color;
    const WalledEntry* entry;
    Byte wall;
    bool forward;
    bool opened;

    void applyMove();
};
