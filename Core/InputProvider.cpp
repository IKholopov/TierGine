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
#include <InputProvider.h>

#include <Time.h>
#include <assert.h>
#include <limits>

namespace TierGine {

KeyListener::KeyListener(std::function<void (int)> action, InputProvider& provider, float timeout) :
    Listener(provider),
    action(action),
    lastHit(std::numeric_limits<float>::min()),
    timeout(timeout)
{
}

void KeyListener::OnEvent(int keyAction)
{
    float time = Time::GetTime();
    if(time - lastHit < timeout) {
        return;
    }
    action(keyAction);
    lastHit = time;
}

MouseListener::MouseListener(std::function<void (int, int)> action, InputProvider& provider) :
    Listener(provider), action(action)
{
}

std::unique_ptr<Listener> InputProvider::AddKeyListener(int key, std::function<void(int)> action, float timeout)
{
    if(keySubscribers.find(key) == keySubscribers.end()) {
        keySubscribers.insert({key, std::set<Listener*>()});
    }
    std::set<Listener*>& set = keySubscribers.find(key)->second;
    std::unique_ptr<Listener> listener(new KeyListener(action, *this, timeout));
    set.insert(listener.get());
    return listener;
}

std::unique_ptr<Listener> InputProvider::AddMouseListener(std::function<void(int, int)> action)
{
    std::unique_ptr<Listener> listener(new MouseListener(action, *this));
    mouseSubscribers.insert(listener.get());
    return listener;
}

std::unique_ptr<Listener> InputProvider::AddMouseButtonListener(int key, std::function<void (int)> action, float timeout)
{
    if(mouseButtonSubscribers.find(key) == mouseButtonSubscribers.end()) {
        mouseButtonSubscribers.insert({key, std::set<Listener*>()});
    }
    std::set<Listener*>& set = mouseButtonSubscribers.find(key)->second;
    std::unique_ptr<Listener> listener(new KeyListener(action, *this, timeout));
    set.insert(listener.get());
    return listener;
}

void InputProvider::OnKey(int key, int action)
{
    auto keySubs = keySubscribers.find(key);
    if(keySubs != keySubscribers.end()) {
        std::set<Listener*>& set = keySubs->second;
        for(auto it = set.begin(); it != set.end(); ++it) {
            static_cast<KeyListener*>(*it)->OnEvent(action);
        }
    }
}

void InputProvider::OnMouse(double x, double y)
{
    for(auto it = mouseSubscribers.begin(); it != mouseSubscribers.end(); ++it) {
        static_cast<MouseListener*>(*it)->OnEvent(x, y);
    }
}

void InputProvider::OnMouseButton(int key)
{
    auto keySubs = mouseButtonSubscribers.find(key);
    if(keySubs != mouseButtonSubscribers.end()) {
        std::set<Listener*>& set = keySubs->second;
        for(auto it = set.begin(); it != set.end(); ++it) {
            static_cast<KeyListener*>(*it)->OnEvent(0);
        }
    }
}

void InputProvider::Unsubscribe(Listener* listener)
{
    for(auto keySubs = keySubscribers.begin();
        keySubs != keySubscribers.end(); ++keySubs)
    {
        std::set<Listener*>& set = keySubs->second;
        auto found = set.find(listener);
        if(found != set.end()) {
            set.erase(found);
        }
    }
    auto found = mouseSubscribers.find(listener);
    if(found != mouseSubscribers.end()) {
        mouseSubscribers.erase(found);
    }
}

}
