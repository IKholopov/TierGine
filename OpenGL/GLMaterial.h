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

#include <Material.h>

namespace TierGine {

class GLMaterial : public IMaterial {
public:
    GLMaterial(ITextureSampler* sampler);

    // ITextureSampler interface
    virtual void SetTileMode(const TileMode& mode) override { sampler->SetTileMode(mode); }
    virtual void SetInterpolation(const Interpolation& interpolation) override { sampler->SetInterpolation(interpolation); }
    virtual UniformVariable GetUniform(IPipeline* pipeline) override { return sampler->GetUniform(pipeline); }
    virtual void BindAt(int textureLevelId) override { sampler->BindAt(textureLevelId); }

    // IMaterial interface
    virtual void Activate(IPipeline* pipeline, UniformVariable& textureValue,
                          UniformVariable& normalMapVariable) override;
    virtual MaterialInfo& GetMaterialInfo() override { return info; }
    virtual const MaterialInfo& GetMaterialInfo() const override { return info; }
    virtual void SetTexture(ITexture* texture) override;
    virtual void SetNormalMap(ITexture* normalMap) override;

private:
    MaterialInfo info;
    ITextureSampler* sampler;
    ITexture* texture;
    ITexture* normal;
};

}
