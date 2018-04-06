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

interface IWindow {
    virtual ~IWindow() {}

    virtual void Resize(int width, int height) = 0;
    virtual void Move(int x, int y) = 0;
    virtual void Fullscreen() = 0;
    virtual void Update() = 0;
    virtual void RegularUpdate() = 0;
    virtual bool ToClose() const = 0;
};

}
