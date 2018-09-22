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
#include <vector>
#include <MazePortalGun.h>
#include <Primitives.h>
#include <SimplePipeline.h>
#include <Log.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLInclude.h>

namespace {
class PortalSource: public TG::PhysicsWorld::ICollisionsIterator {
public:
    // ICollisionsIterator interface
    virtual TierGine::ICollisionSource* GetNext() override { return nullptr; }
};
}

MazePortalGun::MazePortalGun(TG::IContext& context, int width, int height,
                             MazePhysicsEngine* engine, TG::IScene* scene):
    blueCube(nullptr),
    blue(nullptr),
    orange(nullptr),
    blueBuffer(context.CreateFramebuffer(width, height)),
    orangeBuffer(context.CreateFramebuffer(width, height)),
    engine(engine),
    width(width),
    height(height)
{
    /*
    blueCamera.SetAspectRatio(16.0f/9.0f);
    blueCamera.SetFOV(120.0f);
    */
    blueCamera.SetOverridenProjection(blueCamera.OriginalProjection());
    auto bluePortal = std::make_unique<MazePortal>(*context.CreateMesh(), &blueCamera, glm::vec3(0.2f, 0.3f, 0.9f));
    blue = bluePortal.get();
    std::unique_ptr<TG::ISceneObject> blueModel = std::move(bluePortal);
    scene->Add(blueModel);

    /*
    orangeCamera.SetAspectRatio(16.0f/9.0f);
    orangeCamera.SetFOV(120.0f);
    */
    orangeCamera.SetOverridenProjection(orangeCamera.OriginalProjection());
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

void MazePortalGun::SetMaterials(TG::IMaterial* bluePortalMaterial,
                                 TG::IMaterial* orangePortalMaterial)
{
    blueMaterial = bluePortalMaterial;
    blueMaterial->SetTexture(blueBuffer->GetTexture());
    blue->SetMaterial(blueMaterial);

    orangeMaterial = orangePortalMaterial;
    orangeMaterial->SetTexture(orangeBuffer->GetTexture());
    orange->SetMaterial(orangeMaterial);
}

void MazePortalGun::RenderPortals(TG::IScene* scene, TG::ICamera* activeCamera)
{
    if(!blue->IsOpened() || !orange->IsOpened()) {
        return;
    }
    auto mat4 = glm::inverse(activeCamera->GetCameraProjections().View);
    mat4[3] -= glm::vec4(mat4[3][0], mat4[3][1], mat4[3][2], 0.0f);

    float sinP, cosP, sinR, cosR;
    sinP = -mat4[2][0];
    cosP = sqrt(1-sinP*sinP);
    if(abs(cosP) > 0.000001f) {
        sinR = mat4[2][1] / cosP;
        cosR = mat4[2][2] / cosP;
    } else {
        sinR = -mat4[1][2];
        cosR = mat4[1][1];
    }
    glm::vec3 rotCalc(atan2(sinR, cosR),M_PI_2- atan2(sinP, cosP), 0.0f);
    TG::Log::Info() << "--------" << std::endl;
    glm::vec2 rotation = activeCamera->GetDirection();
    /*
    TG::Log::Info() << std::to_string(rotation[0]) << " " << std::to_string(rotation[1]) << std::endl;
    TG::Log::Info() << std::to_string(rotCalc[0]) << " " << std::to_string(rotCalc[1]) << " " << std::to_string(rotCalc[2]) << std::endl;
    */
    TG::Log::Info() << std::to_string(sin(rotation[0] + M_PI_2)) << std::endl;
    TG::Log::Info() << std::to_string(sinP) << std::endl;
    TG::Log::Info() << std::to_string(sin(rotation[1] + M_PI_2)) << std::endl;
    TG::Log::Info() << std::to_string(-sinR) << std::endl;
    TG::Log::Info() << "--------" << std::endl;
    {
        glm::mat4x4 virtualCamView = activeCamera->GetCameraProjections().View * blue->GetViewMatrix(true) *
                glm::inverse(orange->GetViewMatrix(false));
        blueCube->SetOverriden(glm::inverse(virtualCamView));

        float distance = glm::length(glm::dot(activeCamera->GetPosition() - blue->GetPosition(), blue->GetDirection()));
        blueCamera.SetOverridenProjection(ClipProjectionMatrix( blueCamera.OriginalProjection(), orange->GetPlane(distance, virtualCamView)));
        blue->GetUVs(activeCamera->GetCameraProjections());

        Binder blueBinder(*blueBuffer);
        scene->SetCamera(&blueCamera);
        blueCamera.SetOverridenPosition(virtualCamView);
        scene->Render();
    }

    {
        glm::mat4x4 virtualCamToWorld = blue->GetViewMatrix(false) *
                glm::inverse(activeCamera->GetCameraProjections().View * orange->GetViewMatrix(true) );
        glm::mat4x4 virtualCamView = glm::inverse(virtualCamToWorld );

        float distance = glm::length(glm::dot(activeCamera->GetPosition() - orange->GetPosition(), orange->GetDirection()));
        orangeCamera.SetOverridenProjection(ClipProjectionMatrix( orangeCamera.OriginalProjection(), blue->GetPlane(distance, virtualCamView)));

        Binder orangeBinder(*orangeBuffer);
        scene->SetCamera(&orangeCamera);
        orangeCamera.SetOverridenPosition(virtualCamView);
        scene->Render();
    }
}

void MazePortalGun::RenderPortalsRecursive(TierGine::IScene* scene, TierGine::ICamera* activeCamera, int level)
{
    if(!blue->IsOpened() || !orange->IsOpened()) {
        return;
    }
    auto proj = activeCamera->GetCameraProjections();
    auto pipeline = blue->GetPipeline();
    std::vector<std::pair<MazePortal*, MazePortal*>> portals({{blue, orange}});
    for(auto portal: portals) {
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        // Disable depth test
        glDisable(GL_DEPTH_TEST);

        // Enable stencil test, to prevent drawing outside
        // region of current portal depth
        glEnable(GL_STENCIL_TEST);

        // Fail stencil test when inside of outer portal
        // (fail where we should be drawing the inner portal)
        glStencilFunc(GL_NOTEQUAL, level, 0xFF);

        // Increment stencil value on stencil fail
        // (on area of inner portal)
        glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);

        // Enable (writing into) all stencil bits
        glStencilMask(0xFF);

        pipeline->GetUniformVariable("modelMatrix").Set(portal.first->GetPositionTransformation());
        pipeline->GetUniformVariable("viewMatrix").Set(proj.View);
        pipeline->GetUniformVariable("projectionMatrix").Set(proj.Projection);
        portal.first->Draw();

        glm::mat4x4 virtualCamToWorld = portal.first->GetViewMatrix(false) *
                glm::inverse(activeCamera->GetCameraProjections().View * portal.second->GetViewMatrix(true) );
        glm::mat4x4 virtualCamView = glm::inverse(virtualCamToWorld );

        PortalCamera cam;
        cam.SetOverridenPosition(virtualCamView);
        float distance = glm::length(glm::dot(activeCamera->GetPosition() - portal.first->GetPosition(), portal.first->GetDirection()));
        cam.SetOverridenProjection(ClipProjectionMatrix( cam.OriginalProjection(), portal.second->GetPlane(distance, virtualCamView)));

        if(level == maxLevel) {
            // Enable color and depth drawing
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);

            // Clear the depth buffer so we don't interfere with stuff
            // outside of this inner portal
            glClear(GL_DEPTH_BUFFER_BIT);

            // Enable the depth test
            // So the stuff we render here is rendered correctly
            glEnable(GL_DEPTH_TEST);

            // Enable stencil test
            // So we can limit drawing inside of the inner portal
            glEnable(GL_STENCIL_TEST);

            // Disable drawing into stencil buffer
            glStencilMask(0x00);

            // Draw only where stencil value == recursionLevel + 1
            // which is where we just drew the new portal
            glStencilFunc(GL_EQUAL, level + 1, 0xFF);

            auto projLoc = cam.GetCameraProjections();
            pipeline->GetUniformVariable("modelMatrix").Set(portal.first->GetPositionTransformation());
            pipeline->GetUniformVariable("viewMatrix").Set(projLoc.View);
            pipeline->GetUniformVariable("projectionMatrix").Set(projLoc.Projection);
            portal.first->Draw();
            scene->Render();
        } else {
            RenderPortalsRecursive(scene, &cam, level + 1 );
        }

        // Disable color and depth drawing
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        // Enable stencil test and stencil drawing
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);

        // Fail stencil test when inside of our newly rendered
        // inner portal
        glStencilFunc(GL_NOTEQUAL, level + 1, 0xFF);

        // Decrement stencil value on stencil fail
        // This resets the incremented values to what they were before,
        // eventually ending up with a stencil buffer full of zero's again
        // after the last (outer) step.
        glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

        // Draw portal into stencil buffer
        pipeline->GetUniformVariable("modelMatrix").Set(portal.first->GetPositionTransformation());
        pipeline->GetUniformVariable("viewMatrix").Set(proj.View);
        pipeline->GetUniformVariable("projectionMatrix").Set(proj.Projection);
        portal.first->Draw();
    }


    // Disable the stencil test and stencil writing
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    // Disable color writing
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // Enable the depth test, and depth writing.
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Make sure we always write the data into the buffer
    glDepthFunc(GL_ALWAYS);

    // Clear the depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    // Draw portals into depth buffer
    for (auto &pair : portals) {
        pipeline->GetUniformVariable("modelMatrix").Set(pair.second->GetPositionTransformation());
        pipeline->GetUniformVariable("viewMatrix").Set(proj.View);
        pipeline->GetUniformVariable("projectionMatrix").Set(proj.Projection);
        pair.second->Draw();
    }


    // Reset the depth function to the default
    glDepthFunc(GL_LESS);

    // Enable stencil test and disable writing to stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    // Draw at stencil >= recursionlevel
    // which is at the current level or higher (more to the inside)
    // This basically prevents drawing on the outside of this level.
    glStencilFunc(GL_LEQUAL, level, 0xFF);

    // Enable color and depth drawing again
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    // And enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Draw scene objects normally, only at recursionLevel
    scene->SetCamera(activeCamera);
    scene->Render();
}

glm::mat4x4 MazePortalGun::ClipProjectionMatrix(const glm::mat4x4& projection,
                                                const glm::vec4& plane)
{
    glm::vec4 q;
    q.x = (sign(plane.x) + projection[2][0]) / projection[0][0];
    q.y = (sign(plane.y) + projection[2][1]) / projection[1][1];
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

std::unique_ptr<TierGine::PhysicsWorld::ICollisionsIterator> MazePortalGun::GetCollisions(TierGine::IPhysicsObject* obj) const
{
    return std::make_unique<PortalSource>();
}


const glm::mat4x4 PortalCamera::GetPositionTransformation() const
{
    return overridenPosition;
}

const TierGine::CameraData PortalCamera::GetCameraProjections() const
{
    TG::CameraData data;
    data.View = overridenPosition;
    data.Projection = overridenProjection;
    return data;
}
