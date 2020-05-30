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

#include <TierGine.CoreDefs.h>
#include <Physics/PhysicsObject.h>
#include <set>
#include <vector>

namespace TierGine {

class PhysicsWorld;

class ImpactSource {
public:
    class ImpactSolver: public ICollisionSolver {
    public:
        ImpactSolver(const glm::vec3& finalMoment, const glm::vec3& finalPos) :
            moment(finalMoment),
            position(finalPos)
        {}
        // ICollisionSolver interface
        virtual void Update(glm::vec3& center, glm::vec3& momentum, glm::vec3& direction) override;

    private:
        glm::vec3 moment;
        glm::vec3 position;
    };

    ImpactSource(PhysicsWorld* world) : world(world) {}
    virtual ~ImpactSource();

    virtual void ApplyImpact( glm::vec3& moment ) = 0;
    virtual void ApplyConstraint( glm::vec3& position ) = 0;

    void OnBind(IPhysicsObject* object) { binded.insert(object); }
    void OnUnind(IPhysicsObject* object) { binded.erase(binded.find(object)); }

    void OnWorldUpdate();

private:
    std::set<IPhysicsObject*> binded;
    PhysicsWorld* world;
};

class PhysicsWorld {
public:
    interface ICollisionsIterator {
        virtual ~ICollisionsIterator() {}
        virtual ICollisionSource* GetNext() = 0;
    };

    virtual ~PhysicsWorld() {}

    void ApplyWorldUpdate();

    void AddFirstOrderImpact(ImpactSource* source) { firstOrder.insert(source); }
    void AddSecondOrderImpact(ImpactSource* source) { secondOrder.insert(source); }

    void RemoveImpact(ImpactSource* source);

    void AddToWorld(IPhysicsObject* obj) { dynamicObjects.insert(obj); }
    void RemoveFromWorld(IPhysicsObject* obj) { dynamicObjects.erase(dynamicObjects.find(obj)); }

    virtual void AddStatic(ICollisionSource* collisionSource) = 0;
    virtual void RemoveStatic(ICollisionSource* collisionSource) = 0;

protected:
    virtual std::unique_ptr<ICollisionsIterator> GetCollisions(IPhysicsObject* obj) = 0;
    virtual void AddCollisionToResolve(std::pair<ICollisionSource*, IPhysicsObject*> collision) = 0;

private:
    std::set<ImpactSource*> firstOrder;
    std::set<ImpactSource*> secondOrder;
    std::set<IPhysicsObject*> dynamicObjects;
};

}
