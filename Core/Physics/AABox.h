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

#include <glm/glm.hpp>

namespace TierGine {

class AABox {
public:
    AABox(const glm::vec3& v1, const glm::vec3& v2);
    AABox(AABox&& other);

    AABox& operator=(AABox&& other);

    bool Collide(const AABox& other) const;
    bool Collide(const glm::vec3& v) const;

    const glm::vec3& Lower() const { return lower; }
    const glm::vec3& Higher() const { return higher; }

private:
    glm::vec3 lower;
    glm::vec3 higher;
};

}
