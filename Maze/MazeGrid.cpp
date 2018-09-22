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
#include <MazeGrid.h>
#include <MazeConstMeshes.h>
#include <cmath>

using namespace MazeConstMeshes;

Grid::Grid(int x, int y, int z) : x(x), y(y), z(z) {
    grid.resize(x);
    for(int i = 0; i < x; ++i) {
        grid[i].resize(y);
        for(int j = 0; j < y; ++j) {
            grid[i][j].resize(z);
        }
    }
    for(int i = 0; i <= y; ++i ) {
        Byte directions = 0;
        if(i > 0) {
            directions |= Ceiling;
        }
        if(i < y) {
            directions |= Floor;
        }
        horizontal.push_back(std::make_unique<HorizontalCell>(glm::vec3(0.5f, i-0.05f, 0.5f),
                                                              glm::vec3(x - 0.5f, i+0.05f, z - 0.5f),
                                                              directions));
    }
}

namespace {

class CollisionsIterator : public TG::PhysicsWorld::ICollisionsIterator {
public:
    CollisionsIterator() : currentPos(0) {}

    void Add(WalledEntry& entry);
    virtual TierGine::ICollisionSource* GetNext() override;

private:
    std::vector<TG::ICollisionSource*> sources;
    int currentPos;
};


class FixedBox : public TG::ICollisionSource {
public:
    FixedBox(const glm::vec3& lower, const glm::vec3& higher) : box(lower, higher) {}
    virtual const TierGine::AABox& GetAABox() const override { return box; }
    virtual std::unique_ptr<TierGine::ICollisionSolver> GetSolverFor(TierGine::IPhysicsObject& object) override;

private:
    TG::AABox box;

    int getClosestPlaneIndex(const glm::vec3& v);
};

void checkClose(float newDist, float& oldDist, int newId, int& id) {
    if(newDist > oldDist) {
        return;
    }
    oldDist = newDist;
    id = newId;
}

}

MeshData::MeshData(const std::initializer_list<TierGine::Tensor>& init):
    v(*init.begin()),
    norm(*(init.begin()+1)),
    uv(*(init.begin()+2))
{
    assert(init.size() == 3);
}

MeshData::MeshData(MeshData&& other):
    v(std::move(other.v)),
    norm(std::move(other.norm)),
    uv(std::move(other.uv))
{
}

void MeshData::Append(MeshData&& other)
{
    v = v.Add(other.v);
    norm = norm.Add(other.norm);
    uv = uv.Add(other.uv);
}

std::unique_ptr<TG::PhysicsWorld::ICollisionsIterator> Grid::GetCollisions(TierGine::IPhysicsObject* obj) const
{
    glm::vec3 center = obj->GetCenter();
    int xPos = center.x;
    int yPos = center.y;
    int zPos = center.z;
    std::unique_ptr<CollisionsIterator> iterator(new CollisionsIterator());
    for(int i = -1; i < 2; ++i) {
        for(int j = -1; j < 2; ++j) {
            if(zPos+j >= 0 && zPos+j < this->ZLength() &&
               xPos+i >= 0 && xPos+i < this->XLength() &&
               yPos >= 0 && yPos < this->YLength())
            {
                iterator->Add(*grid[xPos+i][yPos][zPos+j]);
            }
        }
    }
    return iterator;
}

WallTarget Grid::GetTarget(const glm::vec3& point, const glm::vec3& direction) const
{
    int x = point.x;
    int y = 0;
    int z = point.z;

    Byte selectedWall = 0;
    WalledEntry* entry = grid[x][y][z].get();
    for(Byte wall = FrontWall; wall < Floor; wall <<= 1) {
        auto collisionSource = entry->GetCollisionFor(wall);
        if(collisionSource == nullptr) {
            continue;
        }
        if(collisionSource->GetAABox().Extend(0.01f).Collide(glm::vec3(point.x,0.5f,point.z))) {
            selectedWall = wall;
            break;
        }
    }
    bool forward = false;
    if(selectedWall & (FrontWall | BackWall)) {
        forward = glm::dot(glm::vec3(1.0f, 0.0f, 0.0f), direction) <  0.0f;
    } else {
        forward = glm::dot(glm::vec3(0.0f, 0.0f, 1.0f), direction) <  0.0f;
    }
    return {entry, selectedWall, forward};
}

glm::vec2 WalledEntry::GetDirectionFrom(Byte wall, bool forward) const
{
    if(wall & (FrontWall | BackWall) ) {
        return glm::vec2(M_PI_2, forward ? 0.0f : M_PI);
    }
    if(wall & (RightWall | LeftWall)) {
        return glm::vec2(M_PI_2, forward ? -M_PI_2 : M_PI_2);
    }
    assert(false);
}

glm::vec3 WalledEntry::GetPositionFrom(unsigned char wall, bool forward) const
{
    if(wall & LeftWall) {
        return glm::vec3(GetPosition().x + 0.2f, GetPosition().y + 0.5f, GetPosition().z + (forward ? 0.65f : 0.35f)  );
    }
    if(wall & RightWall) {
        return glm::vec3(GetPosition().x + 0.8f, GetPosition().y + 0.5f, GetPosition().z + (forward ? 0.65f : 0.35f)  );
    }
    if(wall & BackWall) {
        return glm::vec3(GetPosition().x + (forward ? 0.65f : 0.35f), GetPosition().y + 0.5f, GetPosition().z + 0.2f  );
    }
    if(wall & FrontWall) {
        return glm::vec3(GetPosition().x + (forward ? 0.65f : 0.35f), GetPosition().y + 0.5f, GetPosition().z + 0.8f  );
    }
    assert(false);
}

TierGine::Tensor WalledEntry::GetMapData() const
{
    TG::Tensor mesh = TG::CreateTensor(0, 3, std::initializer_list<float>());
    if(directions & (FrontWall && BackWall)) {
        mesh = mesh.Add(TG::CreateTensor(6, 3, {
                                             GetPosition().x + 0.6f, GetPosition().z, -1.0f,
                                             GetPosition().x + 0.6f, GetPosition().z + 1.0f, -1.0f,
                                             GetPosition().x + 0.4f, GetPosition().z, -1.0f,
                                             GetPosition().x + 0.4f, GetPosition().z, -1.0f,
                                             GetPosition().x + 0.6f, GetPosition().z + 1.0f, -1.0f,
                                             GetPosition().x + 0.4f, GetPosition().z + 1.0f, -1.0f,
                                         }));
    } else {
        if(directions & FrontWall) {
            mesh = mesh.Add(TG::CreateTensor(6, 3, {
                                                 GetPosition().x + 0.6f, GetPosition().z + 0.4f, -1.0f,
                                                 GetPosition().x + 0.6f, GetPosition().z + 1.0f, -1.0f,
                                                 GetPosition().x + 0.4f, GetPosition().z + 0.4f, -1.0f,
                                                 GetPosition().x + 0.4f, GetPosition().z + 0.4f, -1.0f,
                                                 GetPosition().x + 0.6f, GetPosition().z + 1.0f, -1.0f,
                                                 GetPosition().x + 0.4f, GetPosition().z + 1.0f, -1.0f,
                                             }));
        }
        if(directions & BackWall) {
            mesh = mesh.Add(TG::CreateTensor(6, 3, {
                                                 GetPosition().x + 0.6f, GetPosition().z, -1.0f,
                                                 GetPosition().x + 0.6f, GetPosition().z + 0.6f, -1.0f,
                                                 GetPosition().x + 0.4f, GetPosition().z, -1.0f,
                                                 GetPosition().x + 0.4f, GetPosition().z, -1.0f,
                                                 GetPosition().x + 0.6f, GetPosition().z + 0.6f, -1.0f,
                                                 GetPosition().x + 0.4f, GetPosition().z + 0.6f, -1.0f,
                                             }));
        }
    }
    if(directions & RightWall) {
        mesh = mesh.Add(TG::CreateTensor(6, 3, {
                                             GetPosition().x + 1.0f, GetPosition().z + 0.4f, -1.0f,
                                             GetPosition().x + 1.0f, GetPosition().z + 0.6f, -1.0f,
                                             GetPosition().x + 0.4f, GetPosition().z + 0.4f, -1.0f,
                                             GetPosition().x + 0.4f, GetPosition().z + 0.4f, -1.0f,
                                             GetPosition().x + 1.0f, GetPosition().z + 0.6f, -1.0f,
                                             GetPosition().x + 0.4f, GetPosition().z + 0.6f, -1.0f,
                                         }));
    }
    if(directions & LeftWall) {
        mesh = mesh.Add(TG::CreateTensor(6, 3, {
                                             GetPosition().x + 0.6f, GetPosition().z + 0.4f, -1.0f,
                                             GetPosition().x + 0.6f, GetPosition().z + 0.6f, -1.0f,
                                             GetPosition().x, GetPosition().z + 0.4f, -1.0f,
                                             GetPosition().x, GetPosition().z + 0.4f, -1.0f,
                                             GetPosition().x + 0.6f, GetPosition().z + 0.6f, -1.0f,
                                             GetPosition().x, GetPosition().z + 0.6f, -1.0f,
                                         }));
    }
    return mesh;
}


void WalledEntry::GenerateCollisions()
{
    const float eps = 0.25f;
    assert(collisions.size() == 0);
    if((directions & LeftWall) != 0) {
        collisions.push_back(std::make_unique<FixedBox>( glm::vec3(GetPosition().x - eps, GetPosition().y - eps, GetPosition().z + 0.4f - eps  ),
                                                         glm::vec3(GetPosition().x + 0.6f + eps, GetPosition().y + 1.0f + eps, GetPosition().z + 0.6f + eps  )));
    }
    if((directions & RightWall) != 0) {
        collisions.push_back(std::make_unique<FixedBox>( glm::vec3(GetPosition().x + 0.4f - eps, GetPosition().y - eps, GetPosition().z + 0.4f - eps  ),
                                                         glm::vec3(GetPosition().x + 1.0f + eps, GetPosition().y + 1.0 + eps, GetPosition().z + 0.6f + eps  )));
    }
    if((directions & BackWall) != 0) {
        collisions.push_back(std::make_unique<FixedBox>( glm::vec3(GetPosition().x + 0.4f - eps, GetPosition().y - eps, GetPosition().z - eps  ),
                                                         glm::vec3(GetPosition().x + 0.6f + eps, GetPosition().y + 1.0 + eps, GetPosition().z + 0.6f + eps  )));
    }
    if((directions & FrontWall) != 0) {
        collisions.push_back(std::make_unique<FixedBox>( glm::vec3(GetPosition().x + 0.4f - eps, GetPosition().y - eps, GetPosition().z + 0.4f - eps  ),
                                                         glm::vec3(GetPosition().x + 0.6f + eps, GetPosition().y + 1.0 + eps, GetPosition().z + 1.0f + eps  )));
    }
}

void WalledEntry::GetCollisionSources(std::vector<TierGine::ICollisionSource*>& sources)
{
    if(collisions.size() == 0) {
        GenerateCollisions();
    }
    for(auto& source: collisions ) {
        sources.push_back(source.get());
    }
}

TG::ICollisionSource* WalledEntry::GetCollisionFor(Byte wall)
{
    if(collisions.size() == 0) {
        GenerateCollisions();
    }
    int index = 0;
    if(directions & LeftWall) {
        if(wall == LeftWall) {
            return collisions[index].get();
        }
        index++;
    }
    if(directions & RightWall) {
        if(wall == RightWall) {
            return collisions[index].get();
        }
        index++;
    }
    if(directions & BackWall) {
        if(wall == BackWall) {
            return collisions[index].get();
        }
        index++;
    }
    if(directions & FrontWall) {
        if(wall == FrontWall) {
            return collisions[index].get();
        }
        index++;
    }
    return nullptr;
}

MeshData WalledEntry::GetMesh() const
{
    Byte walls = this->GetWalls();
    if((walls & (FrontWall | BackWall | LeftWall | RightWall)) == 0) {
        return {TG::Tensor(0, 3, nullptr), TG::Tensor(0, 3, nullptr), TG::Tensor(0, 3, nullptr)};
    }

    TG::Tensor vertices(0, 3, nullptr);
    TG::Tensor normals(0, 3, nullptr);
    TG::Tensor uvs(0, 2, nullptr);

    MeshData result = {vertices, normals, uvs};

    if(walls & FrontWall) {
        result.Append(GetFrontWall(GetPosition()));
    } else {
        result.Append(GetFrontWallStub(GetPosition()));
    }
    if(walls & BackWall) {
        result.Append(GetBackWall(GetPosition()));
    } else {
        result.Append(GetBackWallStub(GetPosition()));
    }
    if(walls & RightWall) {
        result.Append(GetRightWall(GetPosition()));
    } else {
        result.Append(GetRightWallStub(GetPosition()));
    }
    if(walls & LeftWall) {
        result.Append(GetLeftWall(GetPosition()));
    } else {
        result.Append(GetLeftWallStub(GetPosition()));
    }
    return result;
}

void CollisionsIterator::Add(WalledEntry& entry)
{
    assert(currentPos == 0);
    entry.GetCollisionSources(this->sources);
}

TierGine::ICollisionSource* CollisionsIterator::GetNext()
{
    if(currentPos >= sources.size()) {
        return nullptr;
    }
    return sources.at(currentPos++);
}

class ConstSolver : public TG::ICollisionSolver {
public:
    ConstSolver(const glm::vec3& center, const glm::vec3& momentum):centerValue(center), momenutmValue(momentum) {}
    virtual void Update(glm::vec3& center, glm::vec3& momentum, glm::vec3& direction) override { center = this->centerValue; momentum = momenutmValue; }

private:
    glm::vec3 centerValue;
    glm::vec3 momenutmValue;
};

std::unique_ptr<TierGine::ICollisionSolver> FixedBox::GetSolverFor(TierGine::IPhysicsObject& object)
{
    const float eps = 0.0f;
    auto momentum = object.GetMomentum();
    auto center = object.GetCenter();
    if(!GetAABox().Collide(center)) {
        return std::make_unique<ConstSolver>(center, momentum);
    }
    const glm::vec3& lower = GetAABox().Lower();
    const glm::vec3& higher = GetAABox().Higher();
    switch(getClosestPlaneIndex(center)){
    case 0:
        center.x = lower.x - eps;
        momentum.x = 0.0f;
        break;
    case 1:
        center.y = lower.y - eps;
        momentum.y = 0.0f;
        break;
    case 2:
        center.z = lower.z - eps;
        momentum.z = 0.0f;
        break;
    case 3:
        center.x = higher.x + eps;
        momentum.x = 0.0f;
        break;
    case 4:
        center.y = higher.y + eps;
        momentum.y = 0.0f;
        break;
    case 5:
        center.z = higher.z + eps;
        momentum.z = 0.0f;
        break;
    default:
        assert(false);
    };
    return std::make_unique<ConstSolver>(center, momentum);
}

int FixedBox::getClosestPlaneIndex(const glm::vec3& v)
{
    const glm::vec3& lower = GetAABox().Lower();
    const glm::vec3& higher = GetAABox().Higher();
    int index = 0;
    float dist = abs(v.x - lower.x);
    checkClose(abs(v.y - lower.y), dist, 1, index);
    checkClose(abs(v.z - lower.z), dist, 2, index);
    checkClose(abs(v.x - higher.x), dist, 3, index);
    checkClose(abs(v.y - higher.y), dist, 4, index);
    checkClose(abs(v.z - higher.z), dist, 5, index);
    return index;
}

MeshData HorizontalCell::GetMesh() const
{
    Byte walls = this->GetWalls();
    if((walls & (Floor | Ceiling)) == 0) {
        return { TG::Tensor(0, 3, nullptr), TG::Tensor(0, 3, nullptr), TG::Tensor(0, 3, nullptr)};
    }
    TG::Tensor vertices(0, 3, nullptr);
    TG::Tensor normals(0, 3, nullptr);
    TG::Tensor uvs(0, 2, nullptr);
    if( walls & Floor) {
        float xLower = this->GetLower().x;
        float xHigher = this->GetHigher().x;
        float y = this->GetHigher().y;
        float zLower = this->GetLower().z;
        float zHigher = this->GetHigher().z;
        vertices = vertices.Add(TG::CreateTensor(6, 3, {
                                        xLower, y, zHigher,
                                        xLower, y, zLower,
                                        xHigher, y, zHigher,

                                        xLower, y, zLower,
                                        xHigher, y, zLower,
                                        xHigher, y, zHigher
                                    }));
        normals = normals.Add(TG::CreateTensor(6, 3, {
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                               }));
        uvs = uvs.Add(TG::CreateTensor(6, 2, {
                                                0.0f, zHigher,
                                                 0.0f, 0.0f,
                                                 xHigher, zHigher,
                                                 0.0f, 0.0f,
                                                 xHigher, 0.0f,
                                                 xHigher, zHigher,
                                               }));
    }
    if( walls & Ceiling) {
        float xLower = this->GetLower().x;
        float xHigher = this->GetHigher().x;
        float y = this->GetLower().y;
        float zLower = this->GetLower().z;
        float zHigher = this->GetHigher().z;
        vertices = vertices.Add(TG::CreateTensor(6, 3, {
                                        xLower, y, zLower,
                                        xLower, y, zHigher,
                                        xHigher, y, zHigher,

                                        xLower, y, zLower,
                                        xHigher, y, zHigher,
                                        xHigher, y, zLower
                                    }));
        normals = normals.Add(TG::CreateTensor(6, 3, {
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                               }));
        uvs = uvs.Add(TG::CreateTensor(6, 2, {
                                                 0.0f, 0.0f,
                                                 0.0f, zHigher,
                                                 xHigher, zHigher,
                                                 0.0f, 0.0f,
                                                 xHigher, zHigher,
                                                 xHigher, 0.0f
                                               }));
    }
    return { vertices, normals, uvs };
}
