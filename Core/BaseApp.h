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

#include <Initializable.h>

#include <vector>

#include <Log.h>
#include <Window.h>

namespace TierGine {

class BaseApp {
public:
    BaseApp();
    virtual ~BaseApp() {}

    virtual bool InitializeApp();
    virtual void DeinitializeApp();

    virtual TG_Status Main();

protected:
    virtual TG_Status MainLoop() = 0;
    virtual bool ShouldTerminate() const = 0;

    std::vector<std::unique_ptr<IInitializable>>& Initializers() { return initializers; }

private:
    std::vector<std::unique_ptr<IInitializable>> initializers;
    Log baseLog;
    Log errorLog;
};

}
