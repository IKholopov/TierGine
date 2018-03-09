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

#include <assert.h>

namespace TierGine {

void InputProvider::AddKeyListener(int key, InputListener* listener)
{
    if(keySubscribers.find(key) == keySubscribers.end()) {
        keySubscribers.insert({key, std::set<InputListener*>()});
    }
    std::set<InputListener*>& set = keySubscribers.find(key)->second;
    assert(set.find(listener) == set.end());
    set.insert(listener);
}

void InputProvider::AddMouseListener(InputListener* listener)
{
    assert(mouseSubscribers.find(listener) == mouseSubscribers.end());
    mouseSubscribers.insert(listener);
}

void InputProvider::OnKey(int key, int action)
{
    auto keySubs = keySubscribers.find(key);
    if(keySubs != keySubscribers.end()) {
        std::set<InputListener*>& set = keySubs->second;
        for(auto it = set.begin(); it != set.end(); ++it) {
            (*it)->OnKey(action);
        }
    }
}

void InputProvider::OnMouse(double x, double y)
{
    for(auto it = mouseSubscribers.begin(); it != mouseSubscribers.end(); ++it) {
        (*it)->OnMouse(x, y);
    }
}

void InputProvider::Unsubscribe(Listener* listener)
{
    InputListener* inputListener = static_cast<InputListener*>(listener);
    for(auto keySubs = keySubscribers.begin();
        keySubs != keySubscribers.end(); ++keySubs)
    {
        std::set<InputListener*>& set = keySubs->second;
        auto found = set.find(inputListener);
        if(found != set.end()) {
            set.erase(found);
        }
    }
    auto found = mouseSubscribers.find(inputListener);
    if(found != mouseSubscribers.end()) {
        mouseSubscribers.erase(found);
    }
}

}
