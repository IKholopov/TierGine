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

#include <glm/glm.hpp>
#include <vector>

namespace TierGine {

struct LightInfo
{
    glm::vec3 pos; //положение источника света в мировой системе координат (для точечного источника)
    glm::vec3 La; //цвет и интенсивность окружающего света
    glm::vec3 Ld; //цвет и интенсивность диффузного света
    glm::vec3 Ls;
};

typedef std::vector<LightInfo> LightningScheme;

}