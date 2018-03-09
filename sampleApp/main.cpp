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
#include <GLDebugInitializer.h>

class SampleApp : public TierGine::GLBaseApp {
public:
    SampleApp() : GLBaseApp(TierGine::WindowGLFW::Config(1024, 720, "Sample OpenGL App")) {
        //Initializers().push_back(std::make_unique<TierGine::GLDebugInitializer>());
    }
protected:
    virtual TG_Status MainLoop() override;

private:
    bool buffersInitialized = false;
    float val = 0.0;
    TierGine::BasicFpsCamera camera;
    std::unique_ptr<TierGine::BasicModel> cube;
    TierGine::UniformVariable mult;
    TierGine::UniformVariable modelMatrix;
    TierGine::UniformVariable viewMatrix;
    TierGine::UniformVariable projectionMatrix;

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
    modelMatrix.Set(cube->GetPositionTransformation());
    const TierGine::CameraData projections = camera.GetCamera().GetCameraProjections();
    viewMatrix.Set(projections.View);
    projectionMatrix.Set(projections.Projection);
    cube->SetRenderingMode(TierGine::RM_FILL, TierGine::PRS_FRONT);
    cube->Draw();
    return GLBaseApp::MainLoop();
}


void SampleApp::initializeBuffers()
{
    camera.BindToInputProvider(GetInputProvider());
    initializePipeline();

    cube.reset(new TierGine::BasicModel(TierGine::Primitives::CreateCubeMesh(*GetContext(), 2.0f)));
    cube->SetPosition({7.0f, -3.0f, -3.0f});
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
    modelMatrix = pipeline.GetUniformVariable("modelMatrix");
    projectionMatrix = pipeline.GetUniformVariable("projectionMatrix");
    viewMatrix = pipeline.GetUniformVariable("viewMatrix");
    pipeline.Activate();
}
