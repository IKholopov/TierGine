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

#include <Texture.h>
#include <glm/glm.hpp>

namespace TierGine {

struct MaterialInfo
{
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    float shininess;

    MaterialInfo() : Ka(1.0f, 1.0f, 1.0f),
        Kd(1.0f, 1.0f, 1.0f),
        Ks(1.0f, 1.0f, 1.0f),
        shininess(0.0f)
    {}
};

interface IMaterial : public ITextureSampler {
    virtual void Activate(IPipeline* pipeline) = 0;
    virtual MaterialInfo& GetMaterialInfo() = 0;
    virtual const MaterialInfo& GetMaterialInfo() const = 0;
    virtual void SetTexture(ITexture* texture) = 0;
    virtual void SetNormalMap(ITexture* normalMap) = 0;
};

}
