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
#include <fstream>
#include <unordered_map>

namespace TG = TierGine;

class MazeSceneBuilder {
public:
    constexpr static const char* const BlockName = "block";

    MazeSceneBuilder(std::string path);
    TG::IScene* CreateScene(TG::IContext& context,
                            const TG::ICamera& camera,
                            TierGine::IPipeline& defaultPipeline);

private:
    struct Quad {
        glm::vec3 Vertices[4];
        glm::vec3 Normals[2];
    };

    struct Tile {
        std::vector<Quad> Quads;
    };

    std::unordered_map<std::string, std::unique_ptr<Tile>> tiles;
    std::vector<std::vector<std::string>> map;
    int width, height;

    void processTiles(std::ifstream& file);
    bool processTile(std::ifstream& file);
    void processMap(std::ifstream& file);
    bool processQuad(std::ifstream& file, MazeSceneBuilder::Quad& quad);
};
