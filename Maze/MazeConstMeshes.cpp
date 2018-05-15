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
#include <MazeConstMeshes.h>

namespace MazeConstMeshes {

MeshData GetFrontWall(const glm::vec3& position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    auto vertices = TG::CreateTensor(12, 3, {
                                    x + 0.4f, y + 0.0f, z + 0.6f,
                                    x + 0.4f, y + 1.0f, z + 1.0f,
                                    x + 0.4f, y + 0.0f, z + 1.0f,

                                    x + 0.4f, y + 0.0f, z + 0.6f,
                                    x + 0.4f, y + 1.0f, z + 0.6f,
                                    x + 0.4f, y + 1.0f, z + 1.0f,

                                    x + 0.6f, y + 0.0f, z + 0.6f,
                                    x + 0.6f, y + 0.0f, z + 1.0f,
                                    x + 0.6f, y + 1.0f, z + 1.0f,

                                    x + 0.6f, y + 1.0f, z + 0.6f,
                                    x + 0.6f, y + 0.0f, z + 0.6f,
                                    x + 0.6f, y + 1.0f, z + 1.0f
                                });
    auto normals = TG::CreateTensor(12, 3, {
                                   -1.0f, 0.0f, 0.0f,
                                   -1.0f, 0.0f, 0.0f,
                                   -1.0f, 0.0f, 0.0f,
                                   -1.0f, 0.0f, 0.0f,
                                   -1.0f, 0.0f, 0.0f,
                                   -1.0f, 0.0f, 0.0f,

                                    1.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f,
                                    1.0f, 0.0f, 0.0f
                               });
    auto uvs = TG::CreateTensor(12, 2, {
                               0.0f , 0.1f ,
                               1.0f , 0.5f ,
                               0.0f , 0.5f ,
                               0.0f , 0.1f ,
                               1.0f , 0.1f ,
                               1.0f , 0.5f ,

                                0.0f , 0.1f ,
                                0.0f , 0.5f ,
                                1.0f , 0.5f ,
                                1.0f , 0.1f ,
                                0.0f , 0.1f ,
                                1.0f , 0.5f
                           });
    return {vertices, normals, uvs};
}

MeshData GetBackWall(const glm::vec3& position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    auto vertices = TG::CreateTensor(12, 3, {
                                               x + 0.4f, y + 0.0f, z + 0.0f,
                                               x + 0.4f, y + 1.0f, z + 0.4f,
                                               x + 0.4f, y + 0.0f, z + 0.4f,

                                               x + 0.4f, y + 0.0f, z + 0.0f,
                                               x + 0.4f, y + 1.0f, z + 0.0f,
                                               x + 0.4f, y + 1.0f, z + 0.4f,

                                               x + 0.6f, y + 0.0f, z + 0.0f,
                                               x + 0.6f, y + 0.0f, z + 0.4f,
                                               x + 0.6f, y + 1.0f, z + 0.4f,

                                               x + 0.6f, y + 1.0f, z + 0.0f,
                                               x + 0.6f, y + 0.0f, z + 0.0f,
                                               x + 0.6f, y + 1.0f, z + 0.4f
                                            });
    auto normals = TG::CreateTensor(12, 3, {
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,

                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f,
                                                1.0f, 0.0f, 0.0f
                                           });
    auto uvs = TG::CreateTensor(12, 2, {
                                       0.0f , 0.5f ,
                                       1.0f , 0.9f ,
                                       0.0f , 0.9f ,
                                       0.0f , 0.5f ,
                                       1.0f , 0.5f ,
                                       1.0f , 0.9f ,

                                       0.0f , 0.5f ,
                                       0.0f , 0.9f ,
                                       1.0f , 0.9f ,
                                       1.0f , 0.5f ,
                                       0.0f , 0.5f ,
                                       1.0f , 0.9f
                           });
    return {vertices, normals, uvs};
}

MeshData GetRightWall(const glm::vec3& position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    auto vertices = TG::CreateTensor(12, 3, {
                                               x + 1.0f, y + 0.0f, z + 0.4f,
                                               x + 1.0f, y + 1.0f, z + 0.4f,
                                               x + 0.6f, y + 0.0f, z + 0.4f,

                                               x + 0.6f, y + 0.0f, z + 0.4f,
                                               x + 1.0f, y + 1.0f, z + 0.4f,
                                               x + 0.6f, y + 1.0f, z + 0.4f,

                                               x + 1.0f, y + 0.0f, z + 0.6f,
                                               x + 0.6f, y + 0.0f, z + 0.6f,
                                               x + 1.0f, y + 1.0f, z + 0.6f,

                                               x + 1.0f, y + 1.0f, z + 0.6f,
                                               x + 0.6f, y + 0.0f, z + 0.6f,
                                               x + 0.6f, y + 1.0f, z + 0.6f
                                            });
    auto normals = TG::CreateTensor(12, 3, {
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,

                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f
                                           });
    auto uvs = TG::CreateTensor(12, 2, {
                                       0.0f , 0.5f ,
                                       1.0f , 0.5f ,
                                       0.0f , 0.1f ,
                                       0.0f , 0.1f ,
                                       1.0f , 0.5f ,
                                       1.0f , 0.1f ,

                                        0.0f , 0.5f ,
                                        0.0f , 0.1f ,
                                        1.0f , 0.5f ,
                                        1.0f , 0.5f ,
                                        0.0f , 0.1f ,
                                        1.0f , 0.1f
                           });
    return {vertices, normals, uvs};
}

MeshData GetLeftWall(const glm::vec3& position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    auto vertices = TG::CreateTensor(12, 3, {
                                               x + 0.4f, y + 0.0f, z + 0.4f,
                                               x + 0.4f, y + 1.0f, z + 0.4f,
                                               x + 0.0f, y + 0.0f, z + 0.4f,

                                               x + 0.0f, y + 0.0f, z + 0.4f,
                                               x + 0.4f, y + 1.0f, z + 0.4f,
                                               x + 0.0f, y + 1.0f, z + 0.4f,

                                               x + 0.4f, y + 0.0f, z + 0.6f,
                                               x + 0.0f, y + 0.0f, z + 0.6f,
                                               x + 0.4f, y + 1.0f, z + 0.6f,

                                               x + 0.4f, y + 1.0f, z + 0.6f,
                                               x + 0.0f, y + 0.0f, z + 0.6f,
                                               x + 0.0f, y + 1.0f, z + 0.6f
                                            });
    auto normals = TG::CreateTensor(12, 3, {
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,
                                                0.0f, 0.0f, -1.0f,

                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f,
                                                0.0f, 0.0f, 1.0f
                                           });
    auto uvs = TG::CreateTensor(12, 2, {
                                       0.0f , 0.9f ,
                                       1.0f , 0.9f ,
                                       0.0f , 0.5f ,
                                       0.0f , 0.5f ,
                                       1.0f , 0.9f ,
                                       1.0f , 0.5f ,

                                       0.0f , 0.9f ,
                                       0.0f , 0.5f ,
                                       1.0f , 0.9f ,
                                       1.0f , 0.9f ,
                                       0.0f , 0.5f ,
                                       1.0f , 0.5f
                           });
    return {vertices, normals, uvs};
}

/////////////////////////////////////////////////////////////////////////////////

MeshData GetFrontWallStub(const glm::vec3& position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;
    auto vertices = TG::CreateTensor(6, 3, {
                                    x + 0.4f, y + 0.0f, z + 0.6f,
                                    x + 0.4f, y + 1.0f, z + 0.6f,
                                    x + 0.6f, y + 1.0f, z + 0.6f,

                                    x + 0.4f, y + 0.0f, z + 0.6f,
                                    x + 0.6f, y + 1.0f, z + 0.6f,
                                    x + 0.6f, y + 0.0f, z + 0.6f
                                });
    auto normals = TG::CreateTensor(6, 3, {
                                    0.0f, 0.0f, 1.0f,
                                    0.0f, 0.0f, 1.0f,
                                    0.0f, 0.0f, 1.0f,
                                    0.0f, 0.0f, 1.0f,
                                    0.0f, 0.0f, 1.0f,
                                    0.0f, 0.0f, 1.0f
                               });
    auto uvs = TG::CreateTensor(6, 2, {
                                0.0f , -0.1f ,
                                1.0f , -0.1f ,
                                1.0f , 0.1f ,
                                0.0f , -0.1f ,
                                1.0f , 0.1f ,
                                0.0f , 0.1f
                           });
    return {vertices, normals, uvs};
}

MeshData GetBackWallStub(const glm::vec3& position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    auto vertices = TG::CreateTensor(6, 3, {
                                               x + 0.6f, y + 0.0f, z + 0.4f,
                                               x + 0.6f, y + 1.0f, z + 0.4f,
                                               x + 0.4f, y + 0.0f, z + 0.4f,

                                               x + 0.4f, y + 0.0f, z + 0.4f,
                                               x + 0.6f, y + 1.0f, z + 0.4f,
                                               x + 0.4f, y + 1.0f, z + 0.4f,
                                            });
    auto normals = TG::CreateTensor(6, 3, {
                                               0.0f, 0.0f, -1.0f,
                                               0.0f, 0.0f, -1.0f,
                                               0.0f, 0.0f, -1.0f,
                                               0.0f, 0.0f, -1.0f,
                                               0.0f, 0.0f, -1.0f,
                                               0.0f, 0.0f, -1.0f
                                           });
    auto uvs = TG::CreateTensor(6, 2, {
                                       0.0f , 0.1f ,
                                       1.0f , 0.1f ,
                                       0.0f , -0.1f ,
                                       0.0f , -0.1f ,
                                       1.0f , 0.1f ,
                                       1.0f , -0.1f
                           });
    return {vertices, normals, uvs};
}

MeshData GetRightWallStub(const glm::vec3& position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    auto vertices = TG::CreateTensor(6, 3, {
                                               x + 0.6f, y + 0.0f, z + 0.4f,
                                               x + 0.6f, y + 0.0f, z + 0.6f,
                                               x + 0.6f, y + 1.0f, z + 0.6f,

                                               x + 0.6f, y + 1.0f, z + 0.6f,
                                               x + 0.6f, y + 1.0f, z + 0.4f,
                                               x + 0.6f, y + 0.0f, z + 0.4f
                                            });
    auto normals = TG::CreateTensor(6, 3, {
                                               1.0f, 0.0f, 0.0f,
                                               1.0f, 0.0f, 0.0f,
                                               1.0f, 0.0f, 0.0f,
                                               1.0f, 0.0f, 0.0f,
                                               1.0f, 0.0f, 0.0f,
                                               1.0f, 0.0f, 0.0f
                                           });
    auto uvs = TG::CreateTensor(6, 2, {
                                       0.0f , -0.1f ,
                                       0.0f , 0.1f ,
                                       1.0f , 0.1f ,
                                       1.0f , 0.1f ,
                                       1.0f , -0.1f ,
                                       0.0f , -0.1f
                           });
    return {vertices, normals, uvs};
}

MeshData GetLeftWallStub(const glm::vec3& position)
{
    float x = position.x;
    float y = position.y;
    float z = position.z;

    auto vertices = TG::CreateTensor(6, 3, {
                                               x + 0.4f, y + 0.0f, z + 0.6f,
                                               x + 0.4f, y + 0.0f, z + 0.4f,
                                               x + 0.4f, y + 1.0f, z + 0.6f,

                                               x + 0.4f, y + 1.0f, z + 0.6f,
                                               x + 0.4f, y + 0.0f, z + 0.4f,
                                               x + 0.4f, y + 1.0f, z + 0.4f
                                            });
    auto normals = TG::CreateTensor(6, 3, {
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f,
                                               -1.0f, 0.0f, 0.0f
                                           });
    auto uvs = TG::CreateTensor(6, 2, {
                                       0.0f , 0.1f ,
                                       0.0f , -0.1f ,
                                       1.0f , 0.1f ,
                                       1.0f , 0.1f ,
                                       0.0f , -0.1f ,
                                       1.0f , -0.1f
                           });
    return {vertices, normals, uvs};
}

TierGine::Tensor GetFrontWallNormalUVs(bool forward)
{
    if(forward) {
        return TG::CreateTensor(6, 2, {
                                    1.0f , 0.0f ,
                                    0.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                    1.0f , 1.0f ,
                                    1.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                });
    } else {
        return TG::CreateTensor(6, 2, {
                                     0.0f , 0.0f ,
                                     1.0f , 1.0f ,
                                     1.0f , 0.0f ,
                                     0.0f , 0.0f ,
                                     0.0f , 1.0f ,
                                     1.0f , 1.0f ,
                                });
    }
}

TierGine::Tensor GetBackWallNormalUVs(bool forward)
{
    if(forward) {
        return TG::CreateTensor(6, 2, {
                                    1.0f , 0.0f ,
                                    0.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                    1.0f , 1.0f ,
                                    1.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                });
    } else {
        return TG::CreateTensor(6, 2, {
                                    0.0f , 0.0f ,
                                    1.0f , 1.0f ,
                                    1.0f , 0.0f ,
                                    0.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                    1.0f , 1.0f ,
                                });
    }
}

TierGine::Tensor GetRightWallNormalUVs(bool forward)
{
    if(forward) {
        return TG::CreateTensor(6, 2, {
                                    1.0f , 0.0f ,
                                    0.0f , 0.0f ,
                                    1.0f , 1.0f ,
                                    1.0f , 1.0f ,
                                    0.0f , 0.0f ,
                                    0.0f , 1.0f ,


                                });
    } else {
        return TG::CreateTensor(6, 2, {
                                    0.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                    1.0f , 0.0f ,
                                    1.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                    1.0f , 1.0f
                                });
    }
}

TierGine::Tensor GetLeftWallNormalUVs(bool forward)
{
    if(forward) {
        return TG::CreateTensor(6, 2, {
                                    1.0f , 0.0f ,
                                    0.0f , 0.0f ,
                                    1.0f , 1.0f ,
                                    1.0f , 1.0f ,
                                    0.0f , 0.0f ,
                                    0.0f , 1.0f
                                });
    } else {
        return TG::CreateTensor(6, 2, {
                                    0.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                    1.0f , 0.0f ,
                                    1.0f , 0.0f ,
                                    0.0f , 1.0f ,
                                    1.0f , 1.0f
                                });
    }
}

}
