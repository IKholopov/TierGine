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
#include <Physics/AABox.h>

namespace TierGine {


interface IPhysicsObject;

interface ICollisionSolver {
    virtual void Update(glm::vec3& center, glm::vec3& momentum) = 0;
};

interface ICollisionSource {
    virtual ~ICollisionSource() {}
    virtual const AABox& GetAABox() const = 0;
    virtual std::unique_ptr<ICollisionSolver> GetSolverFor(IPhysicsObject& object) = 0;
};

interface IPhysicsObject : public ICollisionSource {
    virtual const glm::vec3& GetCenter() = 0;
    virtual const glm::vec3& GetMomentum() = 0;
    virtual void ApplyWorldUpdate(ICollisionSolver* solver) = 0;
};

}
