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

#include <mutex>

#include <Camera.h>

namespace TierGine {

class FreeCamera : public ICamera {
public:
    FreeCamera();

    // IWorldObject interface
    virtual const glm::mat4x4 GetPositionTransformation() const override;
    virtual const glm::vec3 GetPosition() const override { return position; }
    virtual void SetPosition(const glm::vec3& position) override
    { std::lock_guard<std::mutex> guard(criticalSection); this->position = position; }
    virtual const glm::vec3 GetRotation() const override
    { return glm::vec3(0.0f, direction[0],direction[1]); }
    virtual void SetRotation(const glm::vec3& rotation) override
    { std::lock_guard<std::mutex> guard(criticalSection); direction[0] = rotation[1]; direction[1] = rotation[2]; }
    virtual const glm::vec3 GetScale() const override { return glm::vec3(1.0f); }
    virtual void SetScale(const glm::vec3& scale) override {}

    // ICamera interface
    virtual const glm::vec2& GetDirection() const override { return direction; }
    virtual void SetDirection(const glm::vec2& direction) override { this->direction = direction; }
    virtual float GetFOV() const override { return fov; }
    virtual void SetFOV(float fov) override { this->fov = fov; }
    virtual float GetAspectRatio() const override { return ratio; }
    virtual void SetAspectRatio(float ratio) override { this->ratio = ratio; }
    virtual float GetNearClipping() const override { return nearClipping; }
    virtual void SetNearClipping(float clipping) override { this->nearClipping = clipping; }
    virtual float GetFarClipping() const override { return farClipping; }
    virtual void SetFarClipping(float clipping) override { this->farClipping = clipping; }
    virtual float GetDistanceToPixel(int x, int y, const IContext& context) const override;
    virtual const CameraData GetCameraProjections() const override;


private:
    mutable std::mutex criticalSection;
    glm::vec3 position;
    glm::vec2 direction; //phi, theta
    float fov;
    float ratio;
    float nearClipping;
    float farClipping;
};

}
