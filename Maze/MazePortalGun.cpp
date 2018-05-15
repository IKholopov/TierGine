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
#include <MazePortalGun.h>
#include <Primitives.h>
#include <SimplePipeline.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

MazePortalGun::MazePortalGun(TG::IContext& context, int width, int height,
                             MazePhysicsEngine* engine, TG::IScene* scene):
    blueCube(nullptr),
    blue(nullptr),
    orange(nullptr),
    blueBuffer(context.CreateFramebuffer(width, height)),
    orangeBuffer(context.CreateFramebuffer(width, height)),
    engine(engine)
{
    blueCamera.SetAspectRatio(4.0f/10.0f);
    blueCamera.SetFOV(120.0f);
    auto bluePortal = std::make_unique<MazePortal>(*context.CreateMesh(), &blueCamera, glm::vec3(0.2f, 0.3f, 0.9f));
    blue = bluePortal.get();
    std::unique_ptr<TG::ISceneObject> blueModel = std::move(bluePortal);
    scene->Add(blueModel);

    orangeCamera.SetAspectRatio(4.0f/10.0f);
    orangeCamera.SetFOV(120.0f);
    auto orangePortal = std::make_unique<MazePortal>(*context.CreateMesh(), &orangeCamera, glm::vec3(0.9f, 0.3f, 0.2f));
    orange = orangePortal.get();
    std::unique_ptr<TG::ISceneObject> orangeModel = std::move(orangePortal);
    scene->Add(orangeModel);

    std::unique_ptr<DebugCube> cube = std::make_unique<DebugCube>(context);
    blueCube = cube.get();
    TG::SimplePipeline cubePipe(context, "./res/shaders/rgb.vert.glsl", "./res/shaders/rgb.frag.glsl");
    blueCube->SetPipeline(cubePipe.GetPipeline());
    std::unique_ptr<TG::ISceneObject> cubeModel(cube.release());
    scene->Add(cubeModel);
}

void MazePortalGun::PerformShot(TierGine::ICamera& camera, float distance, Color color)
{
    float phi0 = camera.GetDirection()[0];
    glm::vec3 target;
    glm::vec3 direction = glm::vec3(cos(phi0), 0.0f, sin(phi0));
    target = camera.GetPosition() + direction*distance;

    WallTarget info = engine->GetTarget(target, direction);
    std::string name = "";
    bool appliable = true;
    switch (info.wall) {
    case FrontWall:
        name = "Front";
        break;
    case BackWall:
        name = "Back";
        break;
    case LeftWall:
        name = "Left";
        break;
    case RightWall:
        name = "Right";
        break;
    default:
        appliable = false;
        break;
    }
    if(appliable) {
        switch (color) {
        case C_BLUE:
            blue->SetMoveTo(info.entry, info.wall, info.forward);
            /*
            orangeCamera.SetPosition(info.entry->GetPositionFrom(info.wall, info.forward));
            orangeCamera.SetDirection(info.entry->GetDirectionFrom(info.wall, info.forward));
            */
            break;
        case C_ORANGE:
            orange->SetMoveTo(info.entry, info.wall, info.forward);
            /*
            blueCamera.SetPosition(info.entry->GetPositionFrom(info.wall, info.forward));
            blueCamera.SetDirection(info.entry->GetDirectionFrom(info.wall, info.forward));
            */
            break;
        default:
            assert(false);
            break;
        }
    }
}

void MazePortalGun::SetPipeline(TG::IPipeline* portalPipeline)
{
    auto textureVariable = portalPipeline->GetUniformVariable("portaledTexture");
    blue->SetPipeline(portalPipeline, textureVariable);
    orange->SetPipeline(portalPipeline, textureVariable);
}

void MazePortalGun::SetMaterials(std::unique_ptr<TG::IMaterial>&& bluePortalMaterial,
                                 std::unique_ptr<TG::IMaterial>&& orangePortalMaterial)
{
    blueMaterial = std::move(bluePortalMaterial);
    blueMaterial->SetTexture(blueBuffer->GetTexture());
    blue->SetMaterial(blueMaterial.get());

    orangeMaterial = std::move(orangePortalMaterial);
    orangeMaterial->SetTexture(orangeBuffer->GetTexture());
    orange->SetMaterial(orangeMaterial.get());
}

void MazePortalGun::RenderPortals(TG::IScene* scene, TG::ICamera* activeCamera)
{
    if(!blue->IsOpened() || !orange->IsOpened()) {
        return;
    }
    {

        glm::mat4x4 virtualCamPos = glm::inverse(orange->GetViewMatrix(false) *
                glm::inverse(activeCamera->GetCameraProjections().View * blue->GetViewMatrix(true) ) );
        blueCube->SetOverriden(orange->GetViewMatrix(false) *
                               glm::inverse(activeCamera->GetCameraProjections().View * blue->GetViewMatrix(true) ) );

        blueCamera.SetOverridenProjection(ClipProjectionMatrix( activeCamera->GetCameraProjections().Projection, virtualCamPos * blue->GetPlane()));

        Binder blueBinder(*blueBuffer);
        scene->SetCamera(&blueCamera);
        blueCamera.SetOverridenPosition(virtualCamPos);
        scene->Render();
    }

    {
        glm::mat4x4 virtualCamPos = glm::inverse(blue->GetViewMatrix(false) *
                glm::inverse(activeCamera->GetCameraProjections().View * orange->GetViewMatrix(true) ) );

        orangeCamera.SetOverridenProjection(ClipProjectionMatrix( activeCamera->GetCameraProjections().Projection, virtualCamPos * orange->GetPlane()));

        Binder orangeBinder(*orangeBuffer);
        scene->SetCamera(&orangeCamera);
        orangeCamera.SetOverridenPosition(virtualCamPos);
        scene->Render();
    }
}

glm::mat4x4 MazePortalGun::ClipProjectionMatrix(const glm::mat4x4& projection,
                                                const glm::vec4& plane)
{
    glm::vec4 q;
    q.x = (sign(plane.x) + projection[2][0]) / projection[0][0];
    q.y = (sign(plane.y) + projection[2][1]) / projection[1][2];
    q.z = -1.0f;
    q.w = (1.0f + projection[2][2]) / projection[3][2];

   glm::vec4 c = plane * (2.0f / glm::dot(plane, q));

   glm::mat4x4 clipped = projection;
   clipped[0][2] = c.x;
   clipped[1][2] = c.y;
   clipped[2][2] = c.z + 1.0f;
   clipped[3][2] = c.w;
   return clipped;
}


const glm::mat4x4 PortalCamera::GetPositionTransformation() const
{
    return overridenPosition;
}

const TierGine::CameraData PortalCamera::GetCameraProjections() const
{
    TG::CameraData data;
    data.View = overridenPosition;
    data.Projection = OriginalProjection();// overridenProjection;
    return data;
}
