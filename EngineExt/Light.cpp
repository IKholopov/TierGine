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
#include <Light.h>

#include <Uniform.h>
#include <Pipeline.h>

namespace TierGine {

template<>
void UniformVariable::Set<LightInfo>(const LightInfo& value)
{
    assert(IsValid());
    pipeline->SetUniformVariable(name + ".pos", CreateTensor(value.pos));
    pipeline->SetUniformVariable(name + ".dir", CreateTensor(value.dir));
    pipeline->SetUniformVariable(name + ".La", CreateTensor(value.La));
    pipeline->SetUniformVariable(name + ".Ld", CreateTensor(value.Ld));
    pipeline->SetUniformVariable(name + ".Ls", CreateTensor(value.Ls));
    pipeline->SetUniformVariable(name + ".ang", CreateTensor(value.ang));
}

}
