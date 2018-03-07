/*
   Copyright 2018 Igor Kholopv, All rights reserved.

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

#include <Drawable.h>
#include <WorldObject.h>

namespace TierGine {

interface ISceneObject :
    public IDrawable,
    public IWorldObject
{
};

interface IScene {
    // Takes ownership
    virtual void Add(std::unique_ptr<ISceneObject>& object) = 0;
    virtual void Render() = 0;
    virtual void Update() = 0;
};

}
