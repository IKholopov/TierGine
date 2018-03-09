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

enum TRenderingMode {
    RM_FILL,
    RM_WIRE,
    RM_POINTS
};

enum TPolygonRenderStyle {
    PRS_FRONT,
    PRS_BACK,
    PRS_BOTH
};

interface IRenderer {
    virtual ~IRenderer() {}
    virtual void RenderWithMode(TRenderingMode mode, TPolygonRenderStyle style) const = 0;
};

interface IDrawable {
    virtual ~IDrawable() {}
    virtual void SetRenderingMode(TRenderingMode mode, TPolygonRenderStyle style) = 0;
    virtual void Draw() = 0;
};

}
