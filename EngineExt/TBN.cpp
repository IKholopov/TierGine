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
#include <TBN.h>

#include <glm/glm.hpp>

namespace TierGine {

std::pair<Tensor, Tensor> TBN::CalculateTangentAndBitangent(const Tensor& vertices, const Tensor& uvs)
{
    assert(vertices.GetChannels() == 3);
    assert(uvs.GetChannels() == 2);
    assert(vertices.GetSize() % 3 == 0);
    assert(uvs.GetSize() == vertices.GetSize());
    std::pair<Tensor, Tensor> tbs = { Tensor(0, 3, nullptr), Tensor(0, 3, nullptr)};
    for(int i = 0; i < vertices.GetSize() / 3; ++i) {
        const float* vPtr = vertices.GetPointer() + 3*3*i;
        const float* uvPtr = uvs.GetPointer() + 3*2*i;
        Tensor deltaV = CreateTensor(2, 3, {
                                         vPtr[3]-vPtr[0], vPtr[4]-vPtr[1], vPtr[5]-vPtr[2],
                                         vPtr[6]-vPtr[0], vPtr[7]-vPtr[1], vPtr[8]-vPtr[2]
                                     });
        Tensor deltaUV = CreateTensor(2, 2, {
                                         uvPtr[2] - uvPtr[0], uvPtr[3] - uvPtr[1],
                                         uvPtr[4] - uvPtr[0], uvPtr[5] - uvPtr[1]
                                      });
        const float* dUvPtr = deltaUV.GetPointer();
        float detInv = 1.0f / (dUvPtr[0] * dUvPtr[3] - dUvPtr[1]*dUvPtr[2]);
        glm::vec3 deltaPos0 = deltaV.ToGLM<glm::vec3>(0);
        glm::vec3 deltaPos1 = deltaV.ToGLM<glm::vec3>(1);
        glm::vec3 tan = detInv*(deltaPos0*dUvPtr[3] - deltaPos1*dUvPtr[1]);
        glm::vec3 bitan = detInv*(deltaPos1*dUvPtr[0] - deltaPos0*dUvPtr[2]);
        for(int i = 0; i < 3; ++i) {
            tbs.first = tbs.first.Add(CreateTensor(1, 3, { tan.x, tan.y, tan.z }));
            tbs.second = tbs.second.Add(CreateTensor(1, 3, { bitan.x, bitan.y, bitan.z }));
        }
    }
    return tbs;
}

}
