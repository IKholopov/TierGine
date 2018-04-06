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
    virtual void Update(glm::vec3& center, glm::vec3& momentum) override { center = this->centerValue; momentum = momenutmValue; }

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
