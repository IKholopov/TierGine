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

#include <Texture.h>
#include <Pipeline.h>
#include <GLInclude.h>

namespace TierGine {

class GLTexture: public ITexture {
public:
    GLTexture(IContext& context);
    // ILoadableResource interface
    virtual void LoadFromPath(std::string path) override;

    // ITexture interface
    virtual void Activate() const override;
    virtual IContext& GetContext() const override { return context; }

private:
    int width;
    int height;
    int channels;
    GLuint id;
    IContext& context;
};

class GLTextureSampler: public ITextureSampler {
public:
    GLTextureSampler(IContext& context, std::string unifromName);

    // ITextureSampler interface
    virtual void SetTileMode(const TileMode& mode) override;
    virtual void SetInterpolation(const Interpolation& interpolation) override;
    virtual UniformVariable GetUniform(IPipeline* pipeline) override;

    virtual void BindAt(int textureLevelId) override;

private:
    const std::string uniformName;
    TileMode currentTileMode;
    Interpolation currentInterpolation;
    GLuint id;
    IContext& context;
};

}
