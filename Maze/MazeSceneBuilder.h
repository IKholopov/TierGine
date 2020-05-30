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
#include <MazePortalGun.h>
#include <MazePhysics.h>
#include <Backend.h>
#include <OnScreen.h>
#include <fstream>
#include <unordered_map>

namespace TG = TierGine;

class MazeSceneBuilder {
public:

    struct MaterialFiles {
      std::string TextureName;
      std::string NormalMapName;
    };

    MazeSceneBuilder(std::vector<std::unique_ptr<TG::IMaterial>>& materials,
                     const std::vector<MaterialFiles>& materialFiles,
                     TG::IBackend& backend);
    TG::SimpleScene* CreateSceneAndGrid(TG::IContext& context,
                            const TG::ICamera& camera,
                            TierGine::IPipeline& defaultPipeline);
    MazePhysicsEngine* CreatePhysicsEngine();
    std::unique_ptr<MazePortalGun> CreatePortalGun(TG::IContext& context, MazePhysicsEngine*engine, TG::IScene* scene);
    std::unique_ptr<OnScreen> CreateScreenQuad(TG::IContext& context);
    std::unique_ptr<Map> CreateMap(TG::IContext& context, const Grid* grid);

private:
    std::vector<std::vector<std::string>> map;
    std::unique_ptr<Grid> grid;
    std::vector<std::vector<bool>> collision;

    std::vector<std::unique_ptr<TG::IMaterial>>& materials;
    const std::vector<MaterialFiles>& materialFiles;
    TierGine::IBackend& backend;

    std::vector<TG::IMaterial*> wallMaterials;
    std::unordered_map<std::string, TG::ITexture*> textures;
    TG::IMaterial* floorMaterial;
    TG::IMaterial* ceilingMaterial;

    void loadMaterials(TierGine::ITextureSampler* sampler, TierGine::IContext& context);
    void loadMaterial(TierGine::ITextureSampler* sampler, TierGine::IContext& context,
                      const MaterialFiles& material);
};
