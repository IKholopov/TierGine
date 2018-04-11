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
#include <Material.h>

#include <Uniform.h>
#include <Pipeline.h>

namespace TierGine {

template<>
void UniformVariable::Set<MaterialInfo>(const MaterialInfo& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name + ".Ka", CreateTensor(value.Ka));
    pipeline->SetUniformVariable(name + ".Kd", CreateTensor(value.Kd));
    pipeline->SetUniformVariable(name + ".Ks", CreateTensor(value.Ks));
    pipeline->SetUniformVariable(name + ".shininess", CreateTensor(value.shininess));
}

}

