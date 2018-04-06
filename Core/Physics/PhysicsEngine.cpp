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
#include <Physics/PhysicsEngine.h>

namespace TierGine {

ImpactSource::~ImpactSource()
{
     world->RemoveImpact(this);
}

void ImpactSource::OnWorldUpdate()
{
    for(auto obj: binded) {
        glm::vec3 moment = obj->GetMomentum();
        ApplyImpact( moment );
        glm::vec3 pos = obj->GetCenter();
        ApplyConstraint(pos);
        ImpactSolver solver(moment, pos);
        obj->ApplyWorldUpdate(&solver);
    }
}

void PhysicsWorld::ApplyWorldUpdate()
{
    for(auto first: firstOrder) {
        first->OnWorldUpdate();
    }

    for(auto obj: dynamicObjects) {
        auto potentialCollisions = GetCollisions(obj);
        for(auto collisionSource = potentialCollisions->GetNext();
            collisionSource != nullptr; collisionSource = potentialCollisions->GetNext()) {
            if(obj->GetAABox().Collide(collisionSource->GetAABox())) {
                AddCollisionToResolve(std::make_pair(collisionSource, obj));
            }
        }
    }

    for(auto second: secondOrder) {
        second->OnWorldUpdate();
    }
}

void PhysicsWorld::RemoveImpact(ImpactSource* source)
{
    auto first = firstOrder.find(source);
    if(first != firstOrder.end()) {
        firstOrder.erase(first);
    }

    auto second = secondOrder.find(source);
    if(second != secondOrder.end()) {
        secondOrder.erase(second);
    }
}

void ImpactSource::ImpactSolver::Update(glm::vec3& center, glm::vec3& momentum)
{
    momentum = this->moment;
    center = this->position;
}

}
