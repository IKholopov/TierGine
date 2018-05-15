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

#include <MazePortal.h>
#include <FreeCamera.h>
#include <MazePhysics.h>
#include <Framebuffer.h>
#include <DebugCube.h>

class PortalCamera: public TG::FreeCamera {
public:
    virtual const glm::mat4x4 GetPositionTransformation() const override;
    virtual const TG::CameraData GetCameraProjections() const override;

    void SetOverridenPosition(const glm::mat4x4& position) { overridenPosition = position; }
    void SetOverridenProjection(const glm::mat4x4& projection) { overridenProjection = projection; }

    glm::mat4x4 OriginalPosition() const { return FreeCamera::GetPositionTransformation(); }
    glm::mat4x4 OriginalProjection() const { return FreeCamera::GetCameraProjections().Projection; }

private:
    glm::mat4x4 overridenPosition;
    glm::mat4x4 overridenProjection;
};

class MazePortalGun {
public:
    enum Color {
        C_BLUE,
        C_ORANGE
    };

    MazePortalGun(TG::IContext& context, int width, int height,
                  MazePhysicsEngine* engine, TG::IScene* scene);

    void PerformShot(TG::ICamera& camera, float distance, Color color);
    void SetPipeline(TG::IPipeline* portalPipeline);
    void SetMaterials(std::unique_ptr<TG::IMaterial>&& bluePortalMaterial,
                     std::unique_ptr<TG::IMaterial>&& orangePortalMaterial);

    void RenderPortals(TG::IScene* scene, TG::ICamera* activeCamera);
    glm::mat4x4 ClipProjectionMatrix(const glm::mat4x4& projection, const glm::vec4& plane);

private:
    DebugCube* blueCube;
    PortalCamera blueCamera;
    PortalCamera orangeCamera;
    std::unique_ptr<TG::IMaterial> blueMaterial;
    std::unique_ptr<TG::IMaterial> orangeMaterial;
    MazePortal* blue;
    MazePortal* orange;
    TG::IFramebuffer* blueBuffer;
    TG::IFramebuffer* orangeBuffer;
    MazePhysicsEngine* engine;
};
