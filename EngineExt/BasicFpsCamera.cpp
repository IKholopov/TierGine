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
#include <BasicFpsCamera.h>
#include <GLFW/glfw3.h>

namespace TierGine {

BasicFpsCamera::BasicFpsCamera():
    momentum(0.0f, 0.0f, 0.0f),
    filter(nullptr),
    speed(0.02f),
    previousX(0),
    previousY(0),
    initialized(false)
{
}

void BasicFpsCamera::BindToInputProvider(InputProvider& provider)
{
    listeners.push_back(std::move(provider.AddMouseListener([this](int x, int y){
        this->onMouse(x, y);
    })));
    listeners.push_back(std::move(provider.AddKeyListener(GLFW_KEY_W,
                                                          [this](int action) {
        this->onForward(action);
    })));
    listeners.push_back(std::move(provider.AddKeyListener(GLFW_KEY_S,
                                                          [this](int action) {
        this->onBackward(action);
    })));
    listeners.push_back(std::move(provider.AddKeyListener(GLFW_KEY_D,
                                                          [this](int action) {
        this->onRight(action);
    })));
    listeners.push_back(std::move(provider.AddKeyListener(GLFW_KEY_A,
                                                          [this](int action) {
        this->onLeft(action);
    })));
}

void BasicFpsCamera::UnbindFromInput()
{
    listeners.clear();
    initialized = false;
}

void BasicFpsCamera::onMouse(int x, int y)
{
    if(!initialized) {
        previousX = x;
        previousY = y;
        initialized = true;
        return;
    }
    glm::vec2 direction = cameraView.GetDirection();
    float& phi = direction[0];
    float& theta = direction[1];
    phi += (x - previousX) / 500.0f;
    if(phi > 2 * M_PI) {
        phi -= 2 * M_PI;
    } else if(phi < 0) {
        phi += 2 * M_PI;
    }
    theta += (y - previousY) / 500.0f;
    if(theta < 0.0001f) {
        theta = 0.0001f;
    }
    if(theta > M_PI - 0.0001f) {
        theta = M_PI - 0.0001f;
    }
    previousX = x;
    previousY = y;
    cameraView.SetDirection(direction);
}

void BasicFpsCamera::onForward(int action)
{
    glm::vec2 direction = cameraView.GetDirection();
    float& phi = direction[0];
    float& theta = direction[1];
    momentum = glm::vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));
    if(physicsObject != nullptr) {
        physicsObject->ApplyWorldUpdate(this);
        cameraView.SetPosition(physicsObject->GetCenter());
    }
}

void BasicFpsCamera::onBackward(int action)
{
    glm::vec2 direction = cameraView.GetDirection();
    float& phi = direction[0];
    float& theta = direction[1];
    momentum = -glm::vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));;
    if(physicsObject != nullptr) {
        physicsObject->ApplyWorldUpdate(this);
        cameraView.SetPosition(physicsObject->GetCenter());
    }
}

void BasicFpsCamera::onRight(int action)
{
    glm::vec2 direction = cameraView.GetDirection();
    float& phi = direction[0];
    phi += M_PI_2;
    momentum = glm::vec3(cos(phi), 0.0f, sin(phi));
    if(physicsObject != nullptr) {
        physicsObject->ApplyWorldUpdate(this);
        cameraView.SetPosition(physicsObject->GetCenter());
    }
}

void BasicFpsCamera::onLeft(int action)
{
    glm::vec2 direction = cameraView.GetDirection();
    float& phi = direction[0];
    phi += M_PI_2;
    momentum = -glm::vec3(cos(phi), 0.0f, sin(phi));
    if(physicsObject != nullptr) {
        physicsObject->ApplyWorldUpdate(this);
        cameraView.SetPosition(physicsObject->GetCenter());
    }
}

void BasicFpsCamera::Update(glm::vec3& center, glm::vec3& momentum, glm::vec3& direction)
{
    momentum = this->momentum;
    center += speed * momentum;
}

}
