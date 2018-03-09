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

struct BasicFpsCamera::OnMouseMove : public InputListener {
    OnMouseMove(InputProvider& provider, BasicFpsCamera& camera) :
        InputListener(provider), camera(camera) {}
    BasicFpsCamera& camera;
    virtual void OnKey(int action) override {}
    virtual void OnMouse(double x, double y) override;
};

struct BasicFpsCamera::OnForward: public InputListener {
    OnForward(InputProvider& provider, BasicFpsCamera& camera) :
        InputListener(provider), camera(camera) {}
    BasicFpsCamera& camera;
    virtual void OnKey(int action) override;
    virtual void OnMouse(double x, double y) override {}
};

struct BasicFpsCamera::OnBackward: public InputListener {
    OnBackward(InputProvider& provider, BasicFpsCamera& camera) :
        InputListener(provider), camera(camera) {}
    BasicFpsCamera& camera;
    virtual void OnKey(int action) override;
    virtual void OnMouse(double x, double y) override {}
};

struct BasicFpsCamera::OnRight: public InputListener {
    OnRight(InputProvider& provider, BasicFpsCamera& camera) :
        InputListener(provider), camera(camera) {}
    BasicFpsCamera& camera;
    virtual void OnKey(int action) override;
    virtual void OnMouse(double x, double y) override {}
};

struct BasicFpsCamera::OnLeft: public InputListener {
    OnLeft(InputProvider& provider, BasicFpsCamera& camera) :
        InputListener(provider), camera(camera) {}
    BasicFpsCamera& camera;
    virtual void OnKey(int action) override;
    virtual void OnMouse(double x, double y) override {}
};

BasicFpsCamera::BasicFpsCamera():
    speed(0.1f),
    previousX(0),
    previousY(0),
    initialized(false)
{
}

void BasicFpsCamera::BindToInputProvider(InputProvider& provider)
{
    listeners.push_back(std::make_unique<OnMouseMove>(provider, *this));
    provider.AddMouseListener(listeners.rbegin()->get());
    listeners.push_back(std::make_unique<OnForward>(provider, *this));
    provider.AddKeyListener(GLFW_KEY_W, listeners.rbegin()->get());
    listeners.push_back(std::make_unique<OnBackward>(provider, *this));
    provider.AddKeyListener(GLFW_KEY_S, listeners.rbegin()->get());
    listeners.push_back(std::make_unique<OnRight>(provider, *this));
    provider.AddKeyListener(GLFW_KEY_D, listeners.rbegin()->get());
    listeners.push_back(std::make_unique<OnLeft>(provider, *this));
    provider.AddKeyListener(GLFW_KEY_A, listeners.rbegin()->get());
}

void BasicFpsCamera::OnMouseMove::OnMouse(double x, double y)
{
    if(!camera.initialized) {
        camera.previousX = x;
        camera.previousY = y;
        camera.initialized = true;
        return;
    }
    glm::vec2 direction = camera.cameraView.GetDirection();
    float& phi = direction[0];
    float& theta = direction[1];
    phi += (x - camera.previousX) / 500.0f;
    if(phi > 2 * M_PI) {
        phi -= 2 * M_PI;
    } else if(phi < 0) {
        phi += 2 * M_PI;
    }
    theta += (y - camera.previousY) / 500.0f;
    if(theta < 0.0001f) {
        theta = 0.0001f;
    }
    if(theta > M_PI - 0.0001f) {
        theta = M_PI - 0.0001f;
    }
    camera.previousX = x;
    camera.previousY = y;
    camera.cameraView.SetDirection(direction);
}

void BasicFpsCamera::OnForward::OnKey(int action)
{
    glm::vec3 position = camera.cameraView.GetPosition();
    glm::vec2 direction = camera.cameraView.GetDirection();
    float& phi = direction[0];
    float& theta = direction[1];
    position += camera.speed*glm::vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));
    camera.cameraView.SetPosition(position);
}

void BasicFpsCamera::OnBackward::OnKey(int action)
{
    glm::vec3 position = camera.cameraView.GetPosition();
    glm::vec2 direction = camera.cameraView.GetDirection();
    float& phi = direction[0];
    float& theta = direction[1];
    position -= camera.speed*glm::vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));
    camera.cameraView.SetPosition(position);
}

void BasicFpsCamera::OnRight::OnKey(int action)
{
    glm::vec3 position = camera.cameraView.GetPosition();
    glm::vec2 direction = camera.cameraView.GetDirection();
    float& phi = direction[0];
    phi += M_PI_2;
    float& theta = direction[1];
    position += camera.speed*glm::vec3(cos(phi), 0.0f, sin(phi));
    camera.cameraView.SetPosition(position);
}

void BasicFpsCamera::OnLeft::OnKey(int action)
{
    glm::vec3 position = camera.cameraView.GetPosition();
    glm::vec2 direction = camera.cameraView.GetDirection();
    float& phi = direction[0];
    phi += M_PI_2;
    float& theta = direction[1];
    position -= camera.speed*glm::vec3(cos(phi), 0.0f, sin(phi));
    camera.cameraView.SetPosition(position);
}

}
