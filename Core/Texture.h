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

#include <LoadableResource.h>
#include <Uniform.h>
#include <Shader.h>

interface IContext;

namespace  TierGine {

interface ITexture : public ILoadableResource {
public:
    ITexture() {}
    virtual ~ITexture() {}

    virtual IContext& GetContext() const = 0;
    virtual void Activate() const = 0;

private:
    ITexture(const ITexture&) = delete;
    ITexture& operator=(const ITexture&) = delete;
};

interface ITextureSampler {
    virtual ~ITextureSampler() {}

    enum TileMode {
        TM_Repeat,
        TM_None,
        TM_Invalid
    };

    enum Interpolation {
        I_Linear,
        I_Near,
        I_Invalid
    };

    virtual IContext& GetContext() const = 0;
    virtual void SetTileMode(const TileMode& mode) = 0;
    virtual void SetInterpolation(const Interpolation& interpolation) = 0;
    virtual UniformVariable GetUniform(IPipeline* pipeline) = 0;
    virtual void BindAt(int textureLevelId) = 0;
};

}
