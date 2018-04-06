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

#include <Physics/PhysicsObject.h>

namespace TierGine {

class PhysicsBox : public IPhysicsObject {
public:
    PhysicsBox(const std::initializer_list<float> initalPosition,
               const std::initializer_list<float> initalMomentum, float boxSize):
        box(glm::vec3(*initalPosition.begin(), *(initalPosition.begin()+1), *(initalPosition.begin()+2)) - glm::vec3(boxSize/2),
            glm::vec3(*initalPosition.begin(), *(initalPosition.begin()+1), *(initalPosition.begin()+2)) + glm::vec3(boxSize/2)),
        position(glm::vec3(*initalPosition.begin(), *(initalPosition.begin()+1), *(initalPosition.begin()+2))),
        momentum(glm::vec3(*initalMomentum.begin(), *(initalMomentum.begin()+1), *(initalMomentum.begin()+2))),
        boxRadius(boxSize/2)
    {}

    PhysicsBox(const glm::vec3& initalPosition, const glm::vec3& initalMomentum, float boxSize):
        box(initalPosition - glm::vec3(boxSize/2), initalPosition + glm::vec3(boxSize/2)),
        position(initalPosition),
        momentum(initalMomentum),
        boxRadius(boxSize/2)
    {}

    // ICollisionSource interface
    virtual const AABox& GetAABox() const override
        { return box; }
    virtual std::unique_ptr<ICollisionSolver> GetSolverFor(IPhysicsObject& object) override { return nullptr; }

    // IPhysicsObject interface
    virtual const glm::vec3& GetCenter() override { return position; }
    virtual const glm::vec3& GetMomentum() override { return momentum; }
    virtual void ApplyWorldUpdate(ICollisionSolver* solver) override {
        solver->Update(position, momentum);
        updateBox();
    }

private:
    AABox box;
    glm::vec3 position;
    glm::vec3 momentum;
    float boxRadius;

    void updateBox() { box = std::move(AABox(position - glm::vec3(boxRadius), position + glm::vec3(boxRadius))); }
};

}
