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

#include <SimpleScene.h>
#include <BasicFpsCamera.h>
#include <MazeGrid.h>
#include <Backend.h>
#include <fstream>
#include <unordered_map>

namespace TG = TierGine;

class MazeSceneBuilder {
public:
    constexpr static const char* const BlockName = "block";

    MazeSceneBuilder(std::vector<std::unique_ptr<TG::IMaterial>>& materials, TG::IBackend& backend);
    TG::SimpleScene* CreateSceneAndGrid(TG::IContext& context,
                            const TG::ICamera& camera,
                            TierGine::IPipeline& defaultPipeline);
    TG::PhysicsWorld* CreatePhysicsEngine();

private:
    std::vector<std::vector<std::string>> map;
    std::unique_ptr<Grid> grid;
    std::vector<std::vector<bool>> collision;

    int width, height;
    std::vector<std::unique_ptr<TG::IMaterial>>& materials;
    TierGine::IBackend& backend;

    TG::IMaterial* wallMaterial;
    TG::IMaterial* floorMaterial;

    void loadMaterials(TierGine::ITextureSampler* sampler, TierGine::IContext& context);
};
