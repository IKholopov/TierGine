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
#include <Tensor.h>
#include <Uniform.h>
#include <vector>
#include <unordered_map>

namespace TierGine {

interface IPipeline {
    IPipeline() {}
    virtual ~IPipeline() {}

    virtual IContext& GetContext() const = 0;
    virtual void BindShader(const IShader* shader) = 0;
    virtual void Build() = 0;
    virtual void Activate() const = 0;
    virtual UniformVariable GetUniformVariable(std::string name) const = 0;
    virtual void SetUniformVariable(std::string name, const Tensor value) const = 0;
    virtual void SetUniformVariable(std::string name, int value) const = 0;
    virtual const std::unordered_map<std::string, UniformVariable>& GetUniformVariables() const = 0;

private:
    IPipeline(const IPipeline& other) = delete;
    IPipeline& operator=(const IPipeline& other) = delete;
};

}
