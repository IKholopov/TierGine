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

#include <AppEntry.h>
#include <GLBaseApp.h>
#include <SimplePipeline.h>
#include <Tensor.h>
#include <Primitives.h>
#include <BasicModel.h>
#include <BasicFpsCamera.h>
#include <SimpleScene.h>
#include <GLDebugInitializer.h>
#include <MazeSceneBuilder.h>
#include <PhysicsPrimitives.h>
#include <MazePhysics.h>
#include <Time.h>
#include <iostream>

namespace TG = TierGine;

class MazeApp : public TG::GLBaseApp {
public:
    MazeApp() : GLBaseApp(TG::WindowGLFW::Config(1920, 1080, "Maze", false)), timeout(0.1f), lastHit(0.0f)
    {
        Initializers().push_back(std::make_unique<TierGine::GLDebugInitializer>());
        listeners.push_back(std::move(GetInputProvider().AddKeyListener(GLFW_KEY_Q, [this](int action) {
            this->switchCamera();
        }, 0.5f)));
    }
protected:
    virtual TG_Status MainLoop() override;
    virtual TG_Status RegularUpdate() override;

private:
    enum CamMode {
      FREE, MAZE
    } mode;
    bool initialized = false;
    TG::BasicFpsCamera freeCamera;
    TG::BasicFpsCamera mazeCamera;
    std::unique_ptr<TG::IPipeline> pipeline;
    std::unique_ptr<TG::SimpleScene> scene;
    std::unique_ptr<TG::PhysicsWorld> physics;
    std::unique_ptr<TG::ImpactSource> yImpact;
    std::unique_ptr<CollisionFilter> collisionFilter;
    TG::Listeners listeners;
    float timeout;
    float lastHit;

    void initializeBuffers();
    void initializePipeline();
    void switchCamera();
};

TG_Status MazeApp::MainLoop()
{
    if(!initialized) {
        mazeCamera.GetCamera().SetPosition({-0.5f, 0.5f, 0.5f});
        freeCamera.GetCamera().SetPosition({1.0f, 0.5f, 1.0f});
        freeCamera.BindToInputProvider(GetInputProvider());
        initializePipeline();
        initializeBuffers();
        //mazeCamera.SetMovementFilter(collisionFilter.get());
        initialized = true;
    }
    scene->Render();
    return GLBaseApp::MainLoop();
}

TG_Status MazeApp::RegularUpdate()
{
    if( initialized ) {
        physics->ApplyWorldUpdate();
    }
    return GLBaseApp::RegularUpdate();
}

void MazeApp::initializeBuffers()
{
    MazeSceneBuilder builder("./res/maze/maze.mz");
    scene.reset(builder.CreateSceneAndGrid(*GetContext(), freeCamera.GetCamera(), *pipeline));
    physics.reset(builder.CreatePhysicsEngine());
    collisionFilter.reset(builder.CreateCollisionFilter());
    std::unique_ptr<TG::IPhysicsObject> playerObjectFreeCam(new TG::PhysicsBox({1.0f, 0.5f, 1.0f}, {0.0f, 0.0f,  0.0f}, 0.3f));
    freeCamera.SetPhysicsObject(std::move(playerObjectFreeCam));
    std::unique_ptr<TG::IPhysicsObject> playerObject(new TG::PhysicsBox({1.0f, 0.5f, 1.0f}, {0.0f, 0.0f,  0.0f}, 2.0f));
    mazeCamera.SetPhysicsObject(std::move(playerObject));
    yImpact.reset(new FixedCoords(0.5f, physics.get()));
    yImpact->OnBind(mazeCamera.GetPhysics());
    physics->AddSecondOrderImpact(yImpact.get());
    physics->AddToWorld(mazeCamera.GetPhysics());
}

void MazeApp::initializePipeline()
{
    assert(pipeline.get() == nullptr);
    pipeline.reset(new TG::SimplePipeline(*GetContext(), "./res/shaders/rgb.vert.glsl",
                                          "./res/shaders/rgb.frag.glsl"));
}

void MazeApp::switchCamera()
{
    mode = static_cast<CamMode>((static_cast<int>(mode) + 1) % 2);
    switch (mode) {
    case FREE:
        mazeCamera.UnbindFromInput();
        freeCamera.BindToInputProvider(GetInputProvider());
        scene->SetCamera(&freeCamera.GetCamera());
        break;
    case MAZE:
        freeCamera.UnbindFromInput();
        mazeCamera.BindToInputProvider(GetInputProvider());
        scene->SetCamera(&mazeCamera.GetCamera());
        break;
    default:
        assert(false);
        break;
    }
}

static TG::AppEntry<MazeApp> appEntry;
