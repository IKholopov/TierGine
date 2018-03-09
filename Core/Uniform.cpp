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
    assert(IsValid());
    pipeline->SetUniformVariable(name, value);
}

template<>
void UniformVariable::Set<float>(float& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::vec2>(glm::vec2& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::vec3>(glm::vec3& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::vec4>(glm::vec4& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::mat2>(glm::mat2& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<glm::mat3>(glm::mat3& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name, CreateTensor(value));
}

template<>
void UniformVariable::Set<const glm::mat4>(const glm::mat4& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name, CreateTensor(value));
}

UniformVariable::UniformVariable(const UniformVariable& other) :
    pipeline(other.pipeline),
    type(other.type),
    name(other.name)
{
}

UniformVariable::UniformVariable():
    pipeline(nullptr),
    type(UT_INVALID)
{

}

UniformVariable::UniformVariable(const UniformVariable&& other) :
    pipeline(other.pipeline),
    type(other.type),
    name(other.name)
{
}

UniformVariable& UniformVariable::operator=(const UniformVariable& other)
{
    if(&other != this) {
        pipeline = other.pipeline;
        type = other.type;
        name = other.name;
    }
    return *this;
}

UniformVariable& UniformVariable::operator=(const UniformVariable&& other)
{
    if(&other != this) {
        pipeline = other.pipeline;
        type = other.type;
        name = other.name;
    }
    return *this;
}

}
