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
#include <SimpleScene.h>

namespace TierGine {

SimpleScene::SimpleScene(const ICamera& camera, IPipeline& defaultPipeline) :
    camera(&camera),
    defaultPipeline(defaultPipeline),
    activePipeline(nullptr),
    pipelineInitialized(false)
{
    assertPipelineUniforms( defaultPipeline.GetUniformVariables() );
}

void SimpleScene::Add(std::unique_ptr<ISceneObject>& object)
{
    if(object->GetPipeline() != nullptr) {
        assertPipelineUniforms(object->GetPipeline()->GetUniformVariables());
    }
    objects.push_back(std::move(object));
}

void SimpleScene::Render()
{
    if(activePipeline == nullptr) {
        activatePipeline(&defaultPipeline);
    }
    for(auto obj = objects.begin(); obj != objects.end(); ++obj) {
        IPipeline* objPipeline = (*obj)->GetPipeline();
        if(objPipeline != nullptr && objPipeline != activePipeline) {
            activatePipeline(objPipeline);
        } else if(objPipeline == nullptr) {
            activatePipeline(&defaultPipeline);
        }
        if(!pipelineInitialized) {
            const TierGine::CameraData projections = camera->GetCameraProjections();
            cameraView.Set(projections.View);
            cameraProjection.Set(projections.Projection);
            pipelineInitialized = true;
        }
        modelPosition.Set((*obj)->GetPositionTransformation());
        (*obj)->Draw();
    }
}

void SimpleScene::Update()
{
}

void SimpleScene::assertPipelineUniforms(const std::unordered_map<std::string, UniformVariable>& variables)
{
    assert(variables.find(CameraProjectionName) != variables.end());
    assert(variables.find(CameraViewName) != variables.end());
    assert(variables.find(ModelPositionName) != variables.end());
}

void SimpleScene::activatePipeline(IPipeline* pipeline)
{
    activePipeline = pipeline;
    activePipeline->Activate();
    pipelineInitialized = false;
    cameraView = pipeline->GetUniformVariable(CameraViewName);
    cameraProjection = pipeline->GetUniformVariable(CameraProjectionName);
    modelPosition = pipeline->GetUniformVariable(ModelPositionName);
}



}
