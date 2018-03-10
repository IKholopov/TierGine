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

#include <Subscription.h>
#include <unordered_map>
#include <set>
#include <functional>
#include <vector>

namespace TierGine {

enum TKeyAction {
    KA_PRESS = 0,
    KA_RELEASE,
    KA_REPEAT
};

class InputProvider;

class KeyListener : public Listener {
public:
    KeyListener(std::function<void(int)> action, InputProvider& provider);
    void OnEvent(int keyAction) { action(keyAction); }
private:
    std::function<void(int)> action;
};


class MouseListener : public Listener {
public:
    MouseListener(std::function<void(int, int)> action, InputProvider& provider);
    void OnEvent(int x, int y) { action(x, y); }
private:
    std::function<void(int, int)> action;
};

class InputProvider : public IObservable{
public:
    const std::unordered_map<int, std::set<Listener*> > GetKeySubscribers() const
    { return keySubscribers; }

    std::unique_ptr<Listener> AddKeyListener(int key, std::function<void(int)> action);
    std::unique_ptr<Listener> AddMouseListener(std::function<void(int, int)> action);
    void OnKey(int key, int action);
    void OnMouse(double x, double y);
    virtual void Unsubscribe(Listener* listener) override;

private:
    std::unordered_map<int, std::set<Listener*> > keySubscribers;
    std::set<Listener*> mouseSubscribers;
};

typedef std::vector<std::unique_ptr<Listener>> Listeners;

}
