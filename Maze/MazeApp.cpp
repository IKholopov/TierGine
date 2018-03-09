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
    MazeApp() : GLBaseApp(TG::WindowGLFW::Config(1280, 960, "Maze", false)) {
        Initializers().push_back(std::make_unique<TierGine::GLDebugInitializer>());
    }
protected:
    virtual TG_Status MainLoop() override;

private:
    bool initialized = false;
    TG::BasicFpsCamera camera;
    std::unique_ptr<TG::IPipeline> pipeline;
    std::unique_ptr<TG::IScene> scene;
    std::vector<std::unique_ptr<TG::InputListener>> listenrs;

    void initializeBuffers();
    void initializePipeline();
};

TG_Status MazeApp::MainLoop()
{
    if(!initialized) {
        camera.BindToInputProvider(GetInputProvider());
        initializePipeline();
        initializeBuffers();
        initialized = true;
    }
    scene->Render();
    return GLBaseApp::MainLoop();
}

void MazeApp::initializeBuffers()
{
    MazeSceneBuilder builder("./res/maze/maze.mz");
    scene.reset(builder.CreateScene(*GetContext(), camera.GetCamera(), *pipeline));
}

void MazeApp::initializePipeline()
{
    assert(pipeline.get() == nullptr);
    pipeline.reset(new TG::SimplePipeline(*GetContext(), "./res/shaders/rgb.vert.glsl",
                                          "./res/shaders/rgb.frag.glsl"));
}

static TG::AppEntry<MazeApp> appEntry;
