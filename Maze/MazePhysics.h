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

#include <Physics/PhysicsEngine.h>
#include <MazeGrid.h>

class MazePhysicsEngine : public TG::PhysicsWorld {
public:
    MazePhysicsEngine(std::unique_ptr<const Grid> grid) : grid(std::move(grid)) {}
    MazePhysicsEngine(MazePhysicsEngine&& other) : grid(std::move(other.grid)) {}

    virtual void AddStatic(TierGine::ICollisionSource* collisionSource) override { assert(false); }
    virtual void RemoveStatic(TierGine::ICollisionSource* collisionSource) override { assert(false); }

protected:
    virtual std::unique_ptr<ICollisionsIterator> GetCollisions(TierGine::IPhysicsObject* obj) override;
    virtual void AddCollisionToResolve(std::pair<TierGine::ICollisionSource*, TierGine::IPhysicsObject*> collision) override;

private:
    std::unique_ptr<const Grid> grid;
};

class FixedCoords : public TG::ImpactSource {
public:
    FixedCoords(float y, TG::PhysicsWorld* world) : TG::ImpactSource(world), y(y) {}

    // ImpactSource interface
    virtual void ApplyImpact(glm::vec3& moment) override {}
    virtual void ApplyConstraint(glm::vec3& position) override {
        position.y = y;
    }

private:
    float y;
};
