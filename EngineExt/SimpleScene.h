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

#include <Scene.h>
#include <Camera.h>
#include <Pipeline.h>
#include <Light.h>
#include <Material.h>
#include <vector>

namespace TierGine {

class SimpleScene : public IScene {
public:
    const char* const CameraProjectionName = "projectionMatrix";
    const char* const CameraViewName = "viewMatrix";
    const char* const ModelPositionName = "modelMatrix";
    const char* const LightInfoName = "light";
    const char* const MaterialInfoName = "material";
    const char* const NormalToCameraMatrixName = "normalToCameraMatrix";


    SimpleScene(const ICamera& camera, IPipeline& defaultPipeline);

    void SetCamera(const ICamera* camera) { this->camera = camera; }
    LightningScheme& Light() { return lights; }

    // IScene interface
    virtual void Add(std::unique_ptr<ISceneObject>& object) override;
    virtual void Render() override;
    virtual void Update() override;

private:
    std::vector<std::unique_ptr<ISceneObject>> objects;
    LightningScheme lights;
    MaterialInfo defaultMaterial;

    UniformVariable cameraProjection;
    UniformVariable cameraView;
    //UniformVariable normalToCameraMatrix;
    UniformVariable modelPosition;
    UniformArray lightsVariable;
    UniformVariable materialVariable;

    const ICamera* camera;
    IPipeline& defaultPipeline;
    IPipeline* activePipeline;
    bool pipelineInitialized;

    void assertPipelineUniforms(const std::unordered_map<std::string, UniformVariable>& variables);
    void activatePipeline(IPipeline* pipeline);
};

}
