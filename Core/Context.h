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
#include <Shader.h>
#include <Pipeline.h>

namespace TierGine {

interface IContext {
    virtual ~IContext() {}

    virtual IPipeline* CreatePipeline() = 0;
    virtual void DeletePipeline(IPipeline* pipeline) = 0;

    virtual IShader* CreateShader(IShader::Type shaderType) = 0;
    virtual void BindShader(const IShader* shader, IPipeline* pipeline) = 0;
    virtual void DeleteShader(IShader* shader) = 0;
    virtual TG_Status Activate() = 0;
};

}
