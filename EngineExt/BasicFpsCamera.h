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

#include <FreeCamera.h>

#include <InputProvider.h>
#include <PhysicsPrimitives.h>
#include <vector>

namespace TierGine {

interface IMovementFilter {
    virtual glm::vec3 FilterPosition(glm::vec3& pos, const glm::vec3& prevPos) = 0;
};

class BasicFpsCamera : public ICollisionSolver {
public:
    BasicFpsCamera();
    ICamera& GetCamera() { return cameraView; }
    void SetMovementFilter(IMovementFilter* filter) { this->filter = filter; }
    IPhysicsObject* GetPhysics() { return physicsObject.get(); }
    void SetPhysicsObject(std::unique_ptr<IPhysicsObject>&& object) {
        this->physicsObject = std::move(object);
        cameraView.SetPosition(physicsObject->GetCenter());
    }
    void BindToInputProvider(InputProvider& provider);
    void UnbindFromInput();

private:
    FreeCamera cameraView;
    glm::vec3 momentum;
    IMovementFilter* filter;
    std::unique_ptr<IPhysicsObject> physicsObject;
    float speed;
    int previousX;
    int previousY;
    bool initialized;
    Listeners listeners;

    void onMouse(int x, int y);
    void onForward(int action);
    void onBackward(int action);
    void onRight(int action);
    void onLeft(int action);

    // ICollisionSolver interface for physicsObject
    virtual void Update(glm::vec3& center, glm::vec3& momentum, glm::vec3& direction) override;
};

}
