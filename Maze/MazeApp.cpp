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

namespace TG = TierGine;

class MazeApp : public TG::GLBaseApp {
public:
    MazeApp() : GLBaseApp(TG::WindowGLFW::Config(1920, 1080, "Maze", false)),
      listener(*this, GetInputProvider())
    {
        Initializers().push_back(std::make_unique<TierGine::GLDebugInitializer>());
        GetInputProvider().AddKeyListener(GLFW_KEY_Q, &listener);
    }
protected:
    virtual TG_Status MainLoop() override;

private:
    enum CamMode {
      FREE, MAZE
    } mode;
    bool initialized = false;
    TG::BasicFpsCamera freeCamera;
    TG::BasicFpsCamera mazeCamera;
    std::unique_ptr<TG::IPipeline> pipeline;
    std::unique_ptr<TG::SimpleScene> scene;
    std::unique_ptr<CollisionFilter> collisionFilter;
    std::vector<std::unique_ptr<TG::InputListener>> listenrs;

    void initializeBuffers();
    void initializePipeline();
    void switchCamera();

    //Press Q to switch between cameras
    struct OnSwitchCam : public TG::InputListener {
        OnSwitchCam(MazeApp& app, TG::InputProvider& provider) : InputListener(provider),
            app(app) {}
        virtual void OnKey(int action) override { app.switchCamera(); }
        virtual void OnMouse(double x, double y) override {}
        MazeApp& app;
    } listener;
};

TG_Status MazeApp::MainLoop()
{
    if(!initialized) {
        mazeCamera.GetCamera().SetPosition({-0.5f, 0.5f, 0.5f});
        freeCamera.GetCamera().SetPosition({-0.5f, 0.5f, 0.5f});
        freeCamera.BindToInputProvider(GetInputProvider());
        initializePipeline();
        initializeBuffers();
        mazeCamera.SetMovementFilter(collisionFilter.get());
        initialized = true;
    }
    scene->Render();
    return GLBaseApp::MainLoop();
}

void MazeApp::initializeBuffers()
{
    MazeSceneBuilder builder("./res/maze/maze.mz");
    scene.reset(builder.CreateScene(*GetContext(), freeCamera.GetCamera(), *pipeline));
    collisionFilter.reset(builder.CreateCollisionFilter());
}

void MazeApp::initializePipeline()
{
    assert(pipeline.get() == nullptr);
    pipeline.reset(new TG::SimplePipeline(*GetContext(), "./res/shaders/rgb.vert.glsl",
                                          "./res/shaders/rgb.frag.glsl"));
}

void MazeApp::switchCamera()
{
    static int timeout = 0;
    if(timeout > 0 && timeout < 10) {
        timeout += 1;
        return;
    }
    if(timeout >= 10) {
        timeout = 1;
    }
    timeout += 1;
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
