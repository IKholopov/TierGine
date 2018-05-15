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

#include <MazeGrid.h>

namespace MazeConstMeshes {

MeshData GetFrontWall(const glm::vec3& position);
MeshData GetBackWall(const glm::vec3& position);
MeshData GetRightWall(const glm::vec3& position);
MeshData GetLeftWall(const glm::vec3& position);

MeshData GetFrontWallStub(const glm::vec3& position);
MeshData GetBackWallStub(const glm::vec3& position);
MeshData GetRightWallStub(const glm::vec3& position);
MeshData GetLeftWallStub(const glm::vec3& position);

TG::Tensor GetFrontWallNormalUVs(bool forward);
TG::Tensor GetBackWallNormalUVs(bool forward);
TG::Tensor GetRightWallNormalUVs(bool forward);
TG::Tensor GetLeftWallNormalUVs(bool forward);

}
