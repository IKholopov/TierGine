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

namespace TierGine {

interface IInitializable {
    virtual ~IInitializable() {}

    // Returns true on success, false on failure
    virtual bool Initialize() = 0;
    // Initialize ON NON-MAIN thread
    virtual bool InitializeThread() = 0;
    // Deinitialize on main thread
    virtual void Deinitialize() = 0;
    // Deinitialize ON NON-MAIN threa
    virtual void DeinitializeThread() = 0;

    // Check if initialized
    virtual bool IsInitialized() = 0;
    virtual bool IsInitializedOnThread() = 0;
};

}
