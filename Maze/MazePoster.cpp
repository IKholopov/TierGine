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
#include <MazePoster.h>
#include <MazeConstMeshes.h>

using namespace MazeConstMeshes;

static MeshData GetBaseData(const WalledEntry* entry) {
    const Byte& walls = entry->GetWalls();

    if(walls & FrontWall) {
        return GetFrontWall(entry->GetPosition());
    } else if(walls & BackWall) {
        return GetBackWall(entry->GetPosition());
    } else if(walls & RightWall) {
        return GetRightWall(entry->GetPosition());
    } else if(walls & LeftWall) {
        return GetLeftWall(entry->GetPosition());
    }
    assert(false);
}

static TG::Tensor GetNormalUVsData(const WalledEntry* entry, bool forward) {
    const Byte& walls = entry->GetWalls();

    if(walls & FrontWall) {
        return GetFrontWallNormalUVs(forward);
    } else if(walls & BackWall) {
        return GetBackWallNormalUVs(forward);
    } else if(walls & RightWall) {
        return GetRightWallNormalUVs(forward);
    } else if(walls & LeftWall) {
        return GetLeftWallNormalUVs(forward);
    }
    assert(false);
}

MeshData MazePoster::GetMesh()
{
    const float offset = 0.015f;
    MeshData base = GetBaseData(entry);
    auto walls = entry->GetWalls();
    if(forward) {
        auto vs = base.v.Slice(6, 12);
        auto shiftedV = TG::CreateTensor<float>(0, vs.GetChannels(), {});
        for(int i = 0; i < 6; ++i) {
            glm::vec3 shifted = vs.ToGLM<glm::vec3>(i) + glm::vec3((walls & (FrontWall | BackWall) ? 1.0f : 0.0f) * offset, 0.0f,
                                    (walls & (RightWall | LeftWall) ? 1.0f : 0.0f) * offset);
            shiftedV = shiftedV.Add(TG::CreateTensor(1,3, {shifted[0], shifted[1], shifted[2]}));
        }
        return { shiftedV, base.norm.Slice(6, 12),
            GetNormalUVsData(entry, forward)};
    } else {
        auto vs = base.v.Slice(0, 6);
        auto shiftedV = TG::CreateTensor<float>(0, vs.GetChannels(), {});
        for(int i = 0; i < 6; ++i) {
            glm::vec3 shifted = vs.ToGLM<glm::vec3>(i) + glm::vec3((walls & (FrontWall | BackWall) ? 1.0f : 0.0f) * -offset, 0.0f,
                                    (walls & (RightWall | LeftWall) ? 1.0f : 0.0f) * -offset);
            shiftedV = shiftedV.Add(TG::CreateTensor(1,3, {shifted[0], shifted[1], shifted[2]}));
        }
        return { shiftedV, base.norm.Slice(0, 6),
            GetNormalUVsData(entry, forward)};
    }
}
