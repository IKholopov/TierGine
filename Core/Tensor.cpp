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

#include <Tensor.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace TierGine {

Tensor::Tensor(int size, char channels, std::shared_ptr<TierGine::Tensor::DataHolder> data) :
    data(data),
    size(size),
    channels(channels)
{
}

Tensor& Tensor::operator=(const Tensor&& other)
{
    if(&other != this) {
        data = other.data;
        size = other.size;
        channels = other.channels;
    }
    return *this;
}

Tensor Tensor::Add(Tensor other) const
{
    assert(GetChannels() == other.GetChannels());
    assert(GetType() == other.GetType() || GetType() == T_ANY || other.GetType() == T_ANY);
    if(data.get() == nullptr) {
        return Tensor(other.GetSize(), other.GetChannels(), other.data);
    }
    return data->Add(other);
}

template<>
Tensor::Type TensorData<char>::GetType() const { return Tensor::T_BYTE; }

template<>
Tensor::Type TensorData<unsigned char>::GetType() const { return Tensor::T_UBYTE; }

template<>
Tensor::Type TensorData<bool>::GetType() const { return Tensor::T_BOOL; }

template<>
Tensor::Type TensorData<short>::GetType() const { return Tensor::T_SHORT; }

template<>
Tensor::Type TensorData<unsigned short>::GetType() const { return Tensor::T_USHORT; }

template<>
Tensor::Type TensorData<int>::GetType() const { return Tensor::T_INT; }

template<>
Tensor::Type TensorData<unsigned int>::GetType() const { return Tensor::T_UINT; }

template<>
Tensor::Type TensorData<float>::GetType() const { return Tensor::T_FLOAT; }

template<>
Tensor::Type TensorData<double>::GetType() const { return Tensor::T_DOUBLE; }

template <>
Tensor CreateTensor<float>(const float& otherImplementation)
{
    return CreateTensor<float>(1, 1, { otherImplementation });
}

template <>
Tensor CreateTensor<glm::vec2>(const glm::vec2& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 2, false);
    return Tensor(1, 2, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::vec3>(const glm::vec3& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 3, false);
    return Tensor(1, 3, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::vec4>(const glm::vec4& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 4, false);
    return Tensor(1, 4, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::mat2>(const glm::mat2& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 4, false);
    return Tensor(2, 2, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::mat3>(const glm::mat3& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 9, false);
    return Tensor(3, 3, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::mat4>(const glm::mat4& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 16, false);
    return Tensor(4, 4, std::shared_ptr<Tensor::DataHolder>(data));
}

template<>
glm::vec3 Tensor::ToGLM<glm::vec3>(int position) {
    assert(this->GetChannels() == 3);
    assert(position < GetSize());
    const float* ptr = GetPointer() + 3*position;
    return glm::vec3(ptr[0], ptr[1], ptr[2]);
}

}
