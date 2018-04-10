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
#include <GL/gl.h>

namespace TierGine {

SimpleScene::SimpleScene(const ICamera& camera, IPipeline& defaultPipeline) :
    camera(&camera),
    defaultPipeline(defaultPipeline),
    activePipeline(nullptr),
    pipelineInitialized(false)
{
    assertPipelineUniforms( defaultPipeline.GetUniformVariables() );

    defaultMaterial.Ka = glm::vec3(1.0f, 1.0f, 0.0f);
    defaultMaterial.Kd = glm::vec3(1.0f, 1.0f, 0.0f);
    defaultMaterial.Ks = glm::vec3(1.0f, 1.0f, 1.0f);
    defaultMaterial.shininess = 128.0f;
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
    pipelineInitialized = false;
    if(activePipeline == nullptr) {
        activatePipeline(&defaultPipeline);
    }
    for(auto obj = objects.begin(); obj != objects.end(); ++obj) {
        IPipeline* objPipeline = (*obj)->GetPipeline();
        if(objPipeline != nullptr && objPipeline != activePipeline) {
            activatePipeline(objPipeline);
        } else if(objPipeline == nullptr && activePipeline != &defaultPipeline ) {
            activatePipeline(&defaultPipeline);
        }
        const auto pos = (*obj)->GetPositionTransformation();
        const TierGine::CameraData projections = camera->GetCameraProjections();
        if(!pipelineInitialized) {
            cameraView.Set(projections.View);
            cameraProjection.Set(projections.Projection);
            pipelineInitialized = true;
        }
        auto transposed = glm::transpose(glm::inverse(glm::mat3(projections.View * pos)));
        normalToCameraMatrix.Set(transposed);
        for(int i = 0; i < 2; ++i){
            lightsVariable[i].Set(lights[i]);
        }
        materialVariable.Set(defaultMaterial);
        modelPosition.Set(pos);
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
    for(int i = 0; i < 2; ++i) {
        lightsVariable.push_back(pipeline->GetUniformVariable(std::string(LightInfoName) + "[" + std::to_string(i) + "]"));
    }
    materialVariable = pipeline->GetUniformVariable(MaterialInfoName);
    normalToCameraMatrix = pipeline->GetUniformVariable(NormalToCameraMatrixName);
}



}
