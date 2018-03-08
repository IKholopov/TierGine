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

template <>
Tensor CreateTensor<float>(float& otherImplementation)
{
    return CreateTensor<float>(1, 1, { otherImplementation });
}

template <>
Tensor CreateTensor<glm::vec2>(glm::vec2& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 2, false);
    return Tensor(1, 2, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::vec3>(glm::vec3& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 3, false);
    return Tensor(1, 3, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::vec4>(glm::vec4& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 4, false);
    return Tensor(1, 4, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::mat2>(glm::mat2& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 4, false);
    return Tensor(2, 2, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::mat3>(glm::mat3& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 9, false);
    return Tensor(3, 3, std::shared_ptr<Tensor::DataHolder>(data));
}

template <>
Tensor CreateTensor<glm::mat4>(glm::mat4& otherImplementation)
{
    auto data = new TensorData<float>(glm::value_ptr(otherImplementation), 16, false);
    return Tensor(4, 4, std::shared_ptr<Tensor::DataHolder>(data));
}

}
