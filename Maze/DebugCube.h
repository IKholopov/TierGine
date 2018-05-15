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
#include <Primitives.h>

namespace TG = TierGine;

class DebugCube: public TG::BasicModel {
public:
    DebugCube(TG::IContext& context): TG::BasicModel(*TG::Primitives::CreateCubeMesh(context, 0.1f)) {}
    virtual const glm::mat4x4 GetPositionTransformation() const override
        { return overriden; }

    void SetOverriden(const glm::mat4x4& val) { overriden = val; }

private:
    glm::mat4x4 overriden;
};
