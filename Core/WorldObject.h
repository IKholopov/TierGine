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

// Object with coordinates
#pragma once

#include <TierGine.CoreDefs.h>
#include <glm/glm.hpp>

namespace TierGine {

interface IWorldObject {
    virtual ~IWorldObject() {}

    virtual const glm::mat4x4 GetPositionTransformation() const = 0;

    virtual const glm::vec3 GetPosition() const = 0;
    void SetPosition(std::initializer_list<float> position);
    virtual void SetPosition(const glm::vec3& position) = 0;

    virtual const glm::vec3 GetRotation() const = 0;
    void SetRotation(std::initializer_list<float> rotation);
    virtual void SetRotation(const glm::vec3& rotation) = 0;

    virtual const glm::vec3 GetScale() const = 0;
    void SetScale(std::initializer_list<float> scale);
    virtual void SetScale(const glm::vec3& scale) = 0;
};


inline void IWorldObject::SetPosition(std::initializer_list<float> position) {
    assert(position.size() == 3);
    glm::vec3 pos;
    for(int i = 0; i < 3; ++i) {
        pos[i] = *(position.begin()+i);
    }
    SetPosition(pos);
}

inline void IWorldObject::SetRotation(std::initializer_list<float> rotation)
{
    assert(rotation.size() == 3);
    glm::vec3 rot;
    for(int i = 0; i < 3; ++i) {
        rot[i] = *(rotation.begin()+i);
    }
    SetRotation(rot);
}

inline void IWorldObject::SetScale(std::initializer_list<float> scale)
{
    assert(scale.size() == 3);
    glm::vec3 scl;
    for(int i = 0; i < 3; ++i) {
        scl[i] = *(scale.begin()+i);
    }
    SetScale(scl);
}

}
