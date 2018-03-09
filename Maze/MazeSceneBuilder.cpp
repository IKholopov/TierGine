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
#include <MazeSceneBuilder.h>

#include <Log.h>
#include <Mesh.h>
#include <Context.h>
#include <SimpleScene.h>
#include <BasicModel.h>
#include <glm/glm.hpp>

namespace {

glm::vec3 ProcessVec(std::ifstream& file) {
    glm::vec3 vec;
    file >> vec[0] >> vec[1] >> vec[2];
    return vec;
}

void CopyVec3(glm::vec3& from, float* to, int& offset) {
     *(to+offset) = from[0];
     offset += 1;
     *(to+offset) = from[2];
     offset += 1;
     *(to+offset) = from[1];
     offset += 1;
}

}

bool MazeSceneBuilder::processQuad(std::ifstream& file, MazeSceneBuilder::Quad& quad)
{
    std::string tag;
    file >> tag;
    if(tag.compare("<quad>") != 0) {
        return false;
    }
    for(int i = 0; i < 4; ++i) {
        quad.Vertices[i] = ProcessVec(file);
    }
    for(int i = 0; i < 2; ++i) {
        quad.Normals[i] = ProcessVec(file);
    }
    file >> tag;
    assert(tag.compare("</quad>") == 0);
    return true;
}

MazeSceneBuilder::MazeSceneBuilder(std::string path) :
    width(0),
    height(0)
{
    try {
        std::ifstream file(path);
        if(!file.is_open()) {
            throw std::ifstream::failure("Failed to open file");
        }
        std::string tag;
        bool mapProcessed = false;
        while(file.peek() != EOF && ! mapProcessed ) {
            file >> tag;
            if(tag.compare("<tiles>") == 0) {
                processTiles(file);
            } else if(tag.compare("<map>") == 0) {
                processMap(file);
                mapProcessed = true;
            } else {
                assert(false);
            }
        }

    } catch (std::ifstream::failure& e) {
        TG::Log::Error() << "Failed to read maze file " << path << std::endl;
        throw;
    }
}

TG::IScene* MazeSceneBuilder::CreateScene(TG::IContext& context,
                                          const TG::ICamera& camera,
                                          TG::IPipeline& defaultPipeline)
{
    std::unique_ptr<TG::IScene> scene(new TG::SimpleScene(camera, defaultPipeline));
    std::unordered_map<std::string, TG::IMesh*> meshes;
    for(auto tileIt = tiles.begin(); tileIt != tiles.end(); ++tileIt) {
        Tile& tile = *tileIt->second.get();
        TG::IMesh* mesh = context.CreateMesh();
        float* verticesValues = new float[tile.Quads.size()*6*3];
        int positionVertices = 0;
        float* normalsValues = new float[tile.Quads.size()*6*3];
        int positionNormals = 0;
        for(auto quad: tile.Quads) {
            CopyVec3(quad.Vertices[0], verticesValues, positionVertices);
            CopyVec3(quad.Vertices[2], verticesValues, positionVertices);
            CopyVec3(quad.Vertices[1], verticesValues, positionVertices);
            CopyVec3(quad.Vertices[2], verticesValues, positionVertices);
            CopyVec3(quad.Vertices[0], verticesValues, positionVertices);
            CopyVec3(quad.Vertices[3], verticesValues, positionVertices);

            CopyVec3(quad.Normals[0], normalsValues, positionNormals);
            CopyVec3(quad.Normals[0], normalsValues, positionNormals);
            CopyVec3(quad.Normals[0], normalsValues, positionNormals);
            CopyVec3(quad.Normals[1], normalsValues, positionNormals);
            CopyVec3(quad.Normals[1], normalsValues, positionNormals);
            CopyVec3(quad.Normals[1], normalsValues, positionNormals);
        }
        TG::Tensor vertices = TG::CreateTensor<float>(tile.Quads.size()*6, 3, verticesValues);
        TG::Tensor normals = TG::CreateTensor<float>(tile.Quads.size()*6, 3, normalsValues);
        mesh->AddAtribute(0, vertices);
        mesh->AddAtribute(1, normals);
        meshes.insert({tileIt->first, mesh});
    }
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            std::unique_ptr<TG::ISceneObject> model(new TG::BasicModel(*meshes.find(map.at(i).at(j))->second));
            model->SetPosition(glm::vec3(-i, 0.0f, j));
            model->SetRenderingMode(TG::RM_FILL, TG::PRS_FRONT);
            scene->Add(model);
        }
    }
    return scene.release();
}

void MazeSceneBuilder::processTiles(std::ifstream& file)
{
    while(processTile(file))
    {
    }
}

bool MazeSceneBuilder::processTile(std::ifstream& file)
{
    std::string tag;
    file >> tag;
    if(tag.compare("</tiles>") == 0) {
        return false;
    }
    std::string name = tag.substr(tag.find_first_of("<")+1, tag.find_first_of(">")-1);
    assert(name.compare(BlockName) != 0);
    Tile tile;
    Quad quad;
    while(processQuad(file, quad)) {
        tile.Quads.push_back(quad);
    }
    tiles.insert({name, std::make_unique<Tile>(tile)});
    return true;
}

void MazeSceneBuilder::processMap(std::ifstream& file)
{
    std::string tag;
    file >> tag;
    assert(tag.compare("<size>") == 0);
    file >> width >> height;
    file >> tag;
    assert(tag.compare("</size>") == 0);
    tag = "";
    for(int i = 0; i < height; ++i) {
        map.push_back(std::vector<std::string>());
        if(tag.size() == 0) {
            file >> tag;
        }
        for(int j = 0; j < width; ++j) {
            std::string name;
            name = tag.substr(tag.find_first_of("<")+1, tag.find_first_of("/")-tag.find_first_of("<")-1);
            assert(tiles.find(name) != tiles.end() || name.compare(BlockName) == 0);
            tag = tag.substr(tag.find_first_of(">")+1);
            map[i].push_back(name);
            if(tag.size() == 0) {
                file >> tag;
            }
        }
    }
    file >> tag;
    assert(tag.compare("</map>") == 0);
}
