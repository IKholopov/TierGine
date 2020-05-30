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
#include <Log.h>
#include <Primitives.h>
#include <BasicModel.h>
#include <BasicFpsCamera.h>
#include <SimpleScene.h>
#include <GLDebugInitializer.h>

class SampleApp : public TierGine::GLBaseApp {
public:
    SampleApp() : GLBaseApp(TierGine::WindowGLFW::Config(1024, 720, "Sample OpenGL App")) {
        Initializers().push_back(std::make_unique<TierGine::GLDebugInitializer>());
    }
protected:
    virtual TG_Status MainLoop() override;

private:
    bool buffersInitialized = false;
    float val = 0.0;
    TierGine::BasicFpsCamera camera;
    TierGine::UniformVariable mult;
    std::unique_ptr<TierGine::IScene> scene;
    TierGine::Listeners listenrs;

    void initializeBuffers();
    void initializePipeline();
};

static TierGine::AppEntry<SampleApp> appEntry;

TG_Status SampleApp::MainLoop()
{
    if( !buffersInitialized ) {
        initializeBuffers();
        assert(buffersInitialized);
    }
    val += 0.05;
    if(val > 6 * 3.14) {
        val = 0;
    }
    mult.Set(val);
    scene->Render();
    return GLBaseApp::MainLoop();
}

void SampleApp::initializeBuffers()
{
    camera.BindToInputProvider(GetInputProvider());
    initializePipeline();

    TierGine::IMesh& cubeMesh = *TierGine::Primitives::CreateCubeMesh(*GetContext(), 2.0f);
    std::unique_ptr<TierGine::ISceneObject> cube(new TierGine::BasicModel(cubeMesh));
    cube->SetPosition({7.0f, -3.0f, -3.0f});
    listenrs.push_back(GetInputProvider().AddKeyListener(GLFW_KEY_Y, [cube=cube.get()](int) {
        auto rot = cube->GetScale();
        rot[0] += 0.01f;
        cube->SetScale(rot);
    }));
    listenrs.push_back(GetInputProvider().AddKeyListener(GLFW_KEY_U, [cube=cube.get()](int) {
        auto rot = cube->GetScale();
        rot[1] += 0.01f;
        cube->SetScale(rot);
    }));
    listenrs.push_back(GetInputProvider().AddKeyListener(GLFW_KEY_I, [cube=cube.get()](int) {
        auto rot = cube->GetScale();
        rot[2] += 0.01f;
        cube->SetScale(rot);
    }));
    scene->Add(cube);

    for(int i = 0; i < 30; ++i) {
        std::unique_ptr<TierGine::ISceneObject> cube2(new TierGine::BasicModel(cubeMesh));
        cube2->SetPosition({7.0f + i * 5.0f, 3.0f, 3.0f});
        if(i % 2 == 0) {
            cube2->SetRenderingMode(TierGine::RM_WIRE, TierGine::PRS_BOTH);
        }
        if(i % 3 == 0) {
            cube2->SetRenderingMode(TierGine::RM_FILL, TierGine::PRS_BACK);
        }
        scene->Add(cube2);
    }

    buffersInitialized = true;
}

void SampleApp::initializePipeline()
{
    auto context = GetContext();
    assert(context != nullptr);
    TierGine::SimplePipeline pipeline(*context, "./res/shaders/rgb.vert.glsl",
                            "./res/shaders/rgb.frag.glsl");
    auto uniforms = pipeline.GetUniformVariables();
    TierGine::Log::Info() << "Uniforms: " << std::endl;
    for(auto uniformIt = uniforms.begin(); uniformIt != uniforms.end(); ++uniformIt) {
        TierGine::Log::Info() << uniformIt->second.GetName() << std::endl;
    }
    mult = pipeline.GetUniformVariable("mult");
    pipeline.Activate();
    scene.reset(new TierGine::SimpleScene(camera.GetCamera(), *pipeline.GetPipeline()));
}
