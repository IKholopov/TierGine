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
#include <GLMaterial.h>
#include <GLInclude.h>
#include <Pipeline.h>

namespace TierGine {

GLMaterial::GLMaterial(ITextureSampler* sampler):
    sampler(sampler),
    texture(nullptr),
    normal(nullptr)
{
    assert(sampler != 0);
}

void GLMaterial::Activate(IPipeline* pipeline, UniformVariable textureVariable,
                          UniformVariable normalVariable)
{
    if(texture != nullptr) {
        textureVariable.Set<int>(0);
        glActiveTexture(GL_TEXTURE0);
        sampler->BindAt(0);
        texture->Activate();
    }
    if(normal != nullptr && normalVariable.IsValid()) {
        normalVariable.Set<int>(1);
        glActiveTexture(GL_TEXTURE1);
        sampler->BindAt(1);
        normal->Activate();
    }
}

void GLMaterial::SetTexture(ITexture* texture)
{
    assert(texture != nullptr);
    this->texture = texture;
}

void GLMaterial::SetNormalMap(ITexture* normalMap)
{
    assert(normalMap != nullptr);
    this->normal = normalMap;
}


}
