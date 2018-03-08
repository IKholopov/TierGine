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

#include <Uniform.h>

#include <glm/glm.hpp>
#include <Pipeline.h>

namespace TierGine {

template<>
void UniformVariable::Set<Tensor>(Tensor& value)
{
    pipeline.SetUniformVariable(name, value);
}

template<>
void UniformVariable::Set<float>(float& value)
{
    pipeline.SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::vec2>(glm::vec2& value)
{
    pipeline.SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::vec3>(glm::vec3& value)
{
    pipeline.SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::vec4>(glm::vec4& value)
{
    pipeline.SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::mat2>(glm::mat2& value)
{
    pipeline.SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::mat3>(glm::mat3& value)
{
    pipeline.SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::mat4>(glm::mat4& value)
{
    pipeline.SetUniformVariable(name, CreateTensor(value));
}

UniformVariable::UniformVariable(const UniformVariable& other) :
    pipeline(other.pipeline),
    type(other.type),
    name(other.name)
{
}

UniformVariable::UniformVariable(const UniformVariable&& other) :
    pipeline(other.pipeline),
    type(other.type),
    name(other.name)
{
}

}
