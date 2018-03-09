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

namespace TierGine {

enum TKeyAction {
    KA_PRESS = 0,
    KA_RELEASE,
    KA_REPEAT
};

class InputListener;

class InputProvider : public IObservable{
public:
    const std::unordered_map<int, std::set<InputListener*> > GetKeySubscribers() const
    { return keySubscribers; }

    void AddKeyListener(int key, InputListener* listener);
    void AddMouseListener(InputListener* listener);
    void OnKey(int key, int action);
    void OnMouse(double x, double y);
    virtual void Unsubscribe(Listener* listener) override;

private:
    std::unordered_map<int, std::set<InputListener*> > keySubscribers;
    std::set<InputListener*> mouseSubscribers;
};

class InputListener : public Listener {
public:
    InputListener(InputProvider& provider) : Listener(provider) {}
    virtual void OnKey(int action) = 0;
    virtual void OnMouse(double x, double y) = 0;
};

}
