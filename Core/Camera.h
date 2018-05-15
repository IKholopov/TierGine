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

#include <WorldObject.h>
#include <Context.h>

#include<glm/glm.hpp>

namespace TierGine {

struct CameraData {
    CameraData() {}
    CameraData(CameraData&& other) = default;
    CameraData(const CameraData&& other): View(other.View), Projection(other.Projection) {}

    glm::mat4 View;
    glm::mat4 Projection;

    CameraData& operator=(CameraData&& other) = default;
};

interface ICamera : public IWorldObject {
    virtual const glm::vec2& GetDirection() const = 0;
    virtual void SetDirection(const glm::vec2& direction) = 0;

    virtual float GetFOV() const = 0;
    virtual void SetFOV(float fov) = 0;

    virtual float GetAspectRatio() const = 0;
    virtual void SetAspectRatio(float ratio) = 0;

    virtual float GetNearClipping() const = 0;
    virtual void SetNearClipping(float clipping) = 0;

    virtual float GetFarClipping() const = 0;
    virtual void SetFarClipping(float clipping) = 0;

    virtual float GetDistanceToPixel(int x, int y, const IContext& context) const = 0;

    virtual const CameraData GetCameraProjections() const = 0;
};

}
