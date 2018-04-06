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
#include <mutex>
#include <assert.h>

namespace TierGine {

class GLFWInitializer : public IInitializable {
public:
    // IInitializable interface
    virtual bool Initialize() override;
    virtual bool InitializeThread() override;
    virtual void Deinitialize() override;
    virtual void DeinitializeThread() override { assert(initializedThread); }

    virtual bool IsInitialized() override { return initialized; }
    virtual bool IsInitializedOnThread() override { return initializedThread; }

private:
    static bool initialized;
    static std::mutex critical;
    thread_local static bool initializedThread;
};

}
