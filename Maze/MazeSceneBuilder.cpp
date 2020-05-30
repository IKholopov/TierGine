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
#include <SimplePipeline.h>
#include <BasicModel.h>
#include <MazePhysics.h>
#include <TBN.h>
#include <glm/glm.hpp>

#include <iostream>

static const int BufferWidth = 1920;//1024;
static const int BufferHeight = 1080;//2560;
static const int GridSize = 24;

namespace {

void Break(Grid& grid, int i, int j, Byte direction) {
    switch (direction) {
    case LeftWall:
        assert(i > 0);
        grid[i][0][j]->Remove(FrontWall);
        grid[i][0][j+1]->Remove(BackWall);
        break;
    case RightWall:
        assert(i < grid.XLength() - 2);
        grid[i+1][0][j]->Remove(FrontWall);
        grid[i+1][0][j+1]->Remove(BackWall);
        break;
    case BackWall:
        assert(j > 0);
        grid[i+1][0][j]->Remove(LeftWall);
        grid[i][0][j]->Remove(RightWall);
        break;
    case FrontWall:
        assert(j < grid.ZLength() - 2);
        grid[i+1][0][j+1]->Remove(LeftWall);
        grid[i][0][j+1]->Remove(RightWall);
        break;
    default:
        assert(false);
        break;
    }
}

Byte GetAllowed(Grid& grid, int i, int j, std::vector<std::vector<bool>>& accessable) {
    assert(i < grid.XLength() - 1);
    assert(j < grid.ZLength() - 1);
    Byte allowed = 0;
    if(i > 0 && !accessable[i-1][j]) {
        allowed |= LeftWall;
    }
    if(j > 0 && !accessable[i][j-1]) {
        allowed |= BackWall;
    }
    if(i < grid.XLength() - 2 && !accessable[i+1][j]) {
        allowed |= RightWall;
    }
    if(j < grid.ZLength() - 2 && !accessable[i][j+1]) {
        allowed |= FrontWall;
    }
    return allowed;
}

void BreakWalls(Grid& grid, int i, int j, std::vector<std::vector<bool>>& accessable) {
    accessable[i][j] = true;
    while(true) {
        Byte allowed = GetAllowed(grid, i, j, accessable);
        int allowedSize = ((allowed & FrontWall) != 0) + ((allowed & BackWall) != 0) +
                ((allowed & LeftWall) != 0) + ((allowed & RightWall) != 0);
        if(allowedSize == 0) {
            return;
        }
        int dirN = rand() % allowedSize;
        Byte chosen = LeftWall;
        for(int i = 0; i <= dirN; ++i) {
            chosen = NextWall(chosen, allowed);
        }
        int nextI = i;
        int nextJ = j;
        switch (chosen) {
        case LeftWall:
            --nextI;
            break;
        case RightWall:
            ++nextI;
            break;
        case BackWall:
            --nextJ;
            break;
        case FrontWall:
            ++nextJ;
            break;
        default:
            assert(false);
            break;
        }
        Break(grid, i, j, chosen);
        BreakWalls(grid, nextI, nextJ, accessable);
    }
}

void ProcessGrid(Grid& grid) {
    for(int y = 0; y < grid.YLength(); ++y) {
        for(int z = 0; z < grid.ZLength(); ++z) {
            grid[0][y][z]->Remove(LeftWall);
        }
        for(int z = 0; z < grid.ZLength(); ++z) {
            grid[grid.XLength()-1][y][z]->Remove(RightWall);
        }

        for(int x = 0; x < grid.XLength(); ++x) {
            grid[x][y][0]->Remove(BackWall);
        }
        for(int x = 0; x < grid.XLength(); ++x) {
            grid[x][y][grid.ZLength()-1]->Remove(FrontWall);
        }
    }

    std::vector<std::vector<bool>> accessable;
    accessable.resize(grid.XLength() - 1);
    for(int i = 0; i < grid.XLength() - 1; ++i) {
        accessable[i].resize(grid.ZLength() - 1, false);
    }
    BreakWalls(grid, 0, 0, accessable);
}

}

MazeSceneBuilder::MazeSceneBuilder(std::vector<std::unique_ptr<TG::IMaterial>>& materials,
                                   const std::vector<MaterialFiles>& materialFiles,
                                   TierGine::IBackend& backend) :
    materials(materials),
    materialFiles(materialFiles),
    backend(backend),
    floorMaterial(nullptr)
{
}

TierGine::SimpleScene* MazeSceneBuilder::CreateSceneAndGrid(TG::IContext& context,
                                          const TG::ICamera& camera,
                                          TG::IPipeline& defaultPipeline)
{
    loadMaterials(context.CreateTextureSampler("diffuseMap"), context);
    std::vector<MeshData> meshes;
    for(int i = 0; i < wallMaterials.size(); ++i) {
        meshes.push_back({
                             TG::Tensor(0, 3, nullptr),
                             TG::Tensor(0, 3, nullptr),
                             TG::Tensor(0, 2, nullptr)
                         });
    }
    std::unique_ptr<TG::SimpleScene> scene(new TG::SimpleScene(camera, defaultPipeline));
    {
        int n = GridSize;
        grid.reset(new Grid(n, 1, n));
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                (*grid)[i][0][j].reset(new WalledEntry(i, 0, j, FrontWall | BackWall | LeftWall | RightWall ));
            }
        }
        ProcessGrid(*grid.get());
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        std::cout << ms << std::endl;
        srand(ms);
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                auto [v, norm, uv] = (*grid)[i][0][j]->GetMesh();
                int meshIndex = rand() % meshes.size();
                meshes[meshIndex].v = meshes[meshIndex].v.Add(v);
                meshes[meshIndex].norm = meshes[meshIndex].norm.Add(norm);
                meshes[meshIndex].uv = meshes[meshIndex].uv.Add(uv);
            }
        }
        for(int i = 0; i < wallMaterials.size(); ++i) {
            TG::IMesh* mesh = context.CreateMesh();
            std::unique_ptr<TG::BasicModel> basicModel(new TG::BasicModel(*mesh));
            basicModel->SetMaterial(wallMaterials[i]);
            basicModel->LoadFromTensors(meshes[i].v, meshes[i].norm, meshes[i].uv);
            auto [tan, bitan] = TG::TBN::CalculateTangentAndBitangent(meshes[i].v, meshes[i].uv);
            basicModel->AddTangentTensors(tan, bitan);
            std::unique_ptr<TG::ISceneObject> model(basicModel.release());
            model->SetRenderingMode(TG::RM_FILL, TG::PRS_FRONT);
            model->SetPipeline(&defaultPipeline, defaultPipeline.GetUniformVariable("diffuseTex"),
                    defaultPipeline.GetUniformVariable("normalTex"),
                    defaultPipeline.GetUniformVariable("material"));
            scene->Add(model);
        }
    }
    {
        TG::IMesh* mesh = context.CreateMesh();
        TG::Tensor vertices(0, 3, nullptr);
        TG::Tensor normals(0, 3, nullptr);
        TG::Tensor uvs(0, 2, nullptr);

        auto [v, norm, uv] = grid->Horizontal(0)->GetMesh();
        vertices = vertices.Add(v);
        normals = normals.Add(norm);
        uvs = uvs.Add(uv);
        auto [tan, bitan] = TG::TBN::CalculateTangentAndBitangent(vertices, uvs);
        std::unique_ptr<TG::BasicModel> basicModel(new TG::BasicModel(*mesh));
        basicModel->SetMaterial(floorMaterial);
        basicModel->LoadFromTensors(vertices, normals, uvs);
        basicModel->AddTangentTensors(tan, bitan);
        std::unique_ptr<TG::ISceneObject> model(basicModel.release());
        model->SetRenderingMode(TG::RM_FILL, TG::PRS_FRONT);
        model->SetPipeline(&defaultPipeline, defaultPipeline.GetUniformVariable("diffuseTex"),
                            defaultPipeline.GetUniformVariable("normalTex"),
                defaultPipeline.GetUniformVariable("material"));
        scene->Add(model);
    }
    {
        TG::IMesh* mesh = context.CreateMesh();
        TG::Tensor vertices(0, 3, nullptr);
        TG::Tensor normals(0, 3, nullptr);
        TG::Tensor uvs(0, 2, nullptr);

        for(int i = 0; i <= grid->YLength(); ++i) {
            auto [v, norm, uv] = grid->Horizontal(i)->GetMesh();
            vertices = vertices.Add(v);
            normals = normals.Add(norm);
            uvs = uvs.Add(uv);
        }
        auto [tan, bitan] = TG::TBN::CalculateTangentAndBitangent(vertices, uvs);
        std::unique_ptr<TG::BasicModel> basicModel(new TG::BasicModel(*mesh));
        basicModel->SetMaterial(ceilingMaterial);
        basicModel->LoadFromTensors(vertices, normals, uvs);
        basicModel->AddTangentTensors(tan, bitan);
        std::unique_ptr<TG::ISceneObject> model(basicModel.release());
        model->SetRenderingMode(TG::RM_FILL, TG::PRS_FRONT);
        model->SetPipeline(&defaultPipeline, defaultPipeline.GetUniformVariable("diffuseTex"),
                            defaultPipeline.GetUniformVariable("normalTex"),
                    defaultPipeline.GetUniformVariable("material"));
        scene->Add(model);
    }

    TG::LightInfo light;
    light.pos = glm::vec3(1.0f, 0.7f, 1.0f);
    light.dir = glm::vec3(0.0f, 0.0f, 1.0f);
    light.La = glm::vec3(0.8f, 0.8f, 0.8f)*2.0f;
    light.Ld = glm::vec3(0.5f, 0.5f, 0.5f)*2.0f;
    light.Ls = glm::vec3(0.4f, 0.4f, 0.4f)*2.0f;
    light.ang = 2.0f;

    scene->Light().push_back(light);
    light.pos = glm::vec3(5.0f, 0.7f, 5.0f);
    light.dir = glm::vec3(0.0f, 0.0f, 1.0f);
    light.La = glm::vec3(1.0f, 1.0f, 1.0f)*2.0f;
    light.Ld = glm::vec3(0.9f, 0.9f, 0.9f)*2.0f;
    light.Ls = glm::vec3(0.9f, 0.9f, 0.9f)*2.0f;
    light.ang = 0.6f;
    scene->Light().push_back(light);

    return scene.release();
}

MazePhysicsEngine* MazeSceneBuilder::CreatePhysicsEngine()
{
    assert(grid.get() != nullptr);
    return new MazePhysicsEngine(std::move(grid));
}

std::unique_ptr<MazePortalGun> MazeSceneBuilder::CreatePortalGun(TierGine::IContext& context, MazePhysicsEngine* engine,TierGine::IScene* scene)
{
    auto portalGun = std::make_unique<MazePortalGun>(context, BufferWidth, BufferHeight, engine, scene);
    TG::SimplePipeline portalPipeline(context, "./res/shaders/portal.vert.glsl",
            "./res/shaders/portal.frag.glsl");
    TG::ITextureSampler* portalSampler = context.CreateTextureSampler("portaledTexture");
    portalSampler->SetTileMode(TG::ITextureSampler::TM_None);
    materials.push_back(backend.CreateMaterial(portalSampler));
    TG::IMaterial* blueMaterial = materials.rbegin()->get();
    materials.push_back(backend.CreateMaterial(portalSampler));
    TG::IMaterial* orangeMaterial = materials.rbegin()->get();
    portalGun->SetPipeline(portalPipeline.GetPipeline());
    portalGun->SetMaterials(blueMaterial, orangeMaterial);
    return portalGun;
}

std::unique_ptr<OnScreen> MazeSceneBuilder::CreateScreenQuad(TierGine::IContext& context)
{
    auto blueMaterial = materials.rbegin()->get();
    auto quad = std::make_unique<OnScreen>(0, 0,2* 384, 2*216, blueMaterial, context);
    TG::SimplePipeline portalPipeline(context, "./res/shaders/tex.vert.glsl",
            "./res/shaders/tex.frag.glsl");
    quad->SetPipeline(portalPipeline.GetPipeline(), portalPipeline.GetUniformVariable("portaledTexture"));
    return quad;
}

std::unique_ptr<Map> MazeSceneBuilder::CreateMap(TierGine::IContext& context, const Grid* grid)
{
    TG::IMesh* mesh = context.CreateMesh();
    TG::Tensor vertices(0, 3, nullptr);

    std::unique_ptr<Map> mapObject;
    int n = GridSize;
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            auto v = static_cast<const WalledEntry*>((*grid)[i][0][j].get())->GetMapData();
            vertices = vertices.Add(v);
        }
    }
    mesh->AddAtribute(0, vertices);
    int size = 500;
    mapObject.reset(new Map(BufferWidth-size, BufferHeight-size, size, size, mesh, context));
    TG::SimplePipeline pipeline(context, "./res/shaders/mono.vert.glsl",
            "./res/shaders/mono.frag.glsl");
    mapObject->SetRenderingMode(TG::RM_FILL, TG::PRS_BOTH);
    mapObject->SetPipeline(pipeline.GetPipeline(), TG::UniformVariable{});
    return mapObject;
}

void MazeSceneBuilder::loadMaterials(TG::ITextureSampler* sampler, TG::IContext& context)
{
    materials.push_back(backend.CreateMaterial(sampler));
    floorMaterial = materials[0].get();
    auto floorTexture = context.CreateTexture();
    floorTexture->LoadFromPath("res/textures/floor.jpg");
    textures.insert({"res/textures/floor.jpg", floorTexture});
    auto floorNormal = context.CreateTexture();
    floorNormal->LoadFromPath("res/textures/floor_norm.jpg");
    textures.insert({"res/textures/floor_norm.jpg", floorNormal});
    floorMaterial->SetTexture(floorTexture);
    floorMaterial->SetNormalMap(floorNormal);
    floorMaterial->GetMaterialInfo().Ka = glm::vec3(0.8f, 0.8f, 0.8f);
    floorMaterial->GetMaterialInfo().Kd = glm::vec3(0.4f, 0.4f, 0.4f);
    floorMaterial->GetMaterialInfo().Ks = glm::vec3(0.4f, 0.4f, 0.4f);
    floorMaterial->GetMaterialInfo().shininess = 10.0f;
    materials.push_back(backend.CreateMaterial(sampler));
    ceilingMaterial = materials[1].get();
    auto ceilTexture = context.CreateTexture();
    ceilTexture->LoadFromPath("res/textures/ceil.jpg");
    textures.insert({"res/textures/ceil.jpg", ceilTexture});
    auto ceilNormal = context.CreateTexture();
    ceilNormal->LoadFromPath("res/textures/ceil_norm.jpg");
    textures.insert({"res/textures/ceil_norm.jpg", ceilNormal});
    ceilingMaterial->SetTexture(ceilTexture);
    ceilingMaterial->SetNormalMap(ceilNormal);
    ceilingMaterial->GetMaterialInfo().Ka = glm::vec3(0.2f);
    ceilingMaterial->GetMaterialInfo().Kd = glm::vec3(0.3f);
    ceilingMaterial->GetMaterialInfo().Ks = glm::vec3(0.9f);
    ceilingMaterial->GetMaterialInfo().shininess = 128.0f;

    for(auto file = materialFiles.begin(); file != materialFiles.end(); ++file) {
        loadMaterial(sampler, context, *file);
    }
}

void MazeSceneBuilder::loadMaterial(TierGine::ITextureSampler* sampler, TierGine::IContext& context,
                                    const MazeSceneBuilder::MaterialFiles& material)
{

    materials.push_back(backend.CreateMaterial(sampler));
    TG::IMaterial* wallMaterial = materials.rbegin()->get();
    TG::ITexture* wallTexture;
    auto found = textures.find(material.TextureName);
    if(found != textures.end()) {
        wallTexture = found->second;
    } else {
        wallTexture = context.CreateTexture();
        wallTexture->LoadFromPath(material.TextureName);
    }
    TG::ITexture* wallNormal;
    found = textures.find(material.NormalMapName);
    if(found != textures.end()) {
        wallNormal = found->second;
    } else {
        wallNormal = context.CreateTexture();
        wallNormal->LoadFromPath(material.NormalMapName);
    }
    wallMaterial->SetTexture(wallTexture);
    wallMaterial->SetNormalMap(wallNormal);
    wallMaterial->GetMaterialInfo().Ka = glm::vec3(0.8f, 0.8f, 0.8f);
    wallMaterial->GetMaterialInfo().Kd = glm::vec3(0.4f, 0.4f, 0.4f);
    wallMaterial->GetMaterialInfo().Ks = glm::vec3(0.7f, 0.7f, 0.7f);
    wallMaterial->GetMaterialInfo().shininess = 50.0f;
    wallMaterials.push_back(wallMaterial);
}
