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
#include <FreeCamera.h>

#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include <Log.h>

namespace TierGine {

FreeCamera::FreeCamera():
    position(glm::vec3(0.0f, 0.0f, 0.0f)),
    direction(glm::vec2(0.0f, M_PI_2)),
    fov(60.0f),
    ratio(16.0f/9.0f),
    nearClipping(0.1f),
    farClipping(100.0f)
{
}

const glm::mat4x4 FreeCamera::GetPositionTransformation() const {
    return glm::translate(glm::mat4(1.0), position);
}

float FreeCamera::GetDistanceToPixel(int x, int y, const IContext& context) const
{
    CameraData data =  GetCameraProjections();
    float screenDepth = context.GetScreenDepthAt(x, y);
    return data.Projection[3].z /(screenDepth * -2.0f + 1.0f - data.Projection[2].z) * -1.0f;
}

const CameraData FreeCamera::GetCameraProjections() const
{
    std::lock_guard<std::mutex> guard(criticalSection);
    CameraData data;
    float phi = direction[0];
    float theta = direction[1];
    data.View = glm::lookAt(position,
                            position+glm::vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi)),
                            glm::vec3(0.0f, 1.0f, 0.0f));
    data.Projection = glm::perspective(glm::radians(fov), ratio, nearClipping, farClipping);
    return data;
}

}
