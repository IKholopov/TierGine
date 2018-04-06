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
#include <Physics/AABox.h>

namespace {

inline float min(float x, float y) {
    return x < y ? x : y;
}

inline float max(float x, float y) {
    return y < x ? x : y;
}

}

namespace TierGine {

AABox::AABox(const glm::vec3& v1, const glm::vec3& v2)
{
    float eps = 1e-5f;
    if(abs(v1.x - v2.x) < eps && abs(v1.y - v2.y) < eps &&
            abs(v1.z - v2.z) < eps)
    {
        assert(false);
    }
    lower.x = min(v1.x, v2.x);
    higher.x = max(v1.x, v2.x);
    lower.y = min(v1.y, v2.y);
    higher.y = max(v1.y, v2.y);
    lower.z = min(v1.z, v2.z);
    higher.z = max(v1.z, v2.z);
}

AABox::AABox(AABox&& other) :
    lower(other.lower),
    higher(other.higher)
{
}

AABox& AABox::operator=(AABox&& other)
{
    if(&other != this) {
        this->higher = other.higher;
        this->lower = other.lower;
    }
    return *this;
}

bool AABox::Collide(const AABox& other) const
{
    return !(lower.x > other.higher.x || higher.x < other.lower.x ||
                lower.y > other.higher.y || higher.y < other.lower.y ||
             lower.z > other.higher.z || higher.z < other.lower.z);
}

bool AABox::Collide(const glm::vec3& v) const
{
    return !(v.x < lower.x || v.x > higher.x ||
            v.y < lower.y || v.y > higher.y ||
            v.z < lower.z || v.z > higher.z);
}

}
