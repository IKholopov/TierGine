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
#include <MazeSceneBuilder.h>

#include <Log.h>
#include <Mesh.h>
#include <Context.h>
#include <SimpleScene.h>
#include <BasicModel.h>
#include <MazePhysics.h>
#include <glm/glm.hpp>

struct MeshData {
    TG::Tensor v, norm, uv;
};


MeshData GetMeshFromHorizontalCell(const HorizontalCell* entry) {
    Byte walls = entry->GetWalls();
    if((walls & (Floor | Ceiling)) == 0) {
        return { TG::Tensor(0, 3, nullptr), TG::Tensor(0, 3, nullptr), TG::Tensor(0, 3, nullptr)};
    }
    TG::Tensor vertices(0, 3, nullptr);
    TG::Tensor normals(0, 3, nullptr);
    TG::Tensor uvs(0, 2, nullptr);
    if( walls & Floor) {
        float xLower = entry->GetLower().x;
        float xHigher = entry->GetHigher().x;
        float y = entry->GetHigher().y;
        float zLower = entry->GetLower().z;
        float zHigher = entry->GetHigher().z;
        vertices = vertices.Add(TG::CreateTensor(6, 3, {
                                        xLower, y, zHigher,
                                        xLower, y, zLower,
                                        xHigher, y, zHigher,

                                        xLower, y, zLower,
                                        xHigher, y, zLower,
                                        xHigher, y, zHigher
                                    }));
        normals = normals.Add(TG::CreateTensor(6, 3, {
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                                 0.0f, 1.0f, 0.0f,
                                               }));
        uvs = uvs.Add(TG::CreateTensor(6, 2, {
                                                0.0f, zHigher,
                                                 0.0f, 0.0f,
                                                 xHigher, zHigher,
                                                 0.0f, 0.0f,
                                                 xHigher, 0.0f,
                                                 xHigher, zHigher,
                                               }));
    }
    if( walls & Ceiling) {
        float xLower = entry->GetLower().x;
        float xHigher = entry->GetHigher().x;
        float y = entry->GetLower().y;
        float zLower = entry->GetLower().z;
        float zHigher = entry->GetHigher().z;
        vertices = vertices.Add(TG::CreateTensor(6, 3, {
                                        xLower, y, zLower,
                                        xLower, y, zHigher,
                                        xHigher, y, zHigher,

                                        xLower, y, zLower,
                                        xHigher, y, zHigher,
                                        xHigher, y, zLower
                                    }));
        normals = normals.Add(TG::CreateTensor(6, 3, {
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                                 0.0f, -1.0f, 0.0f,
                                               }));
        uvs = uvs.Add(TG::CreateTensor(6, 2, {
                                                 0.0f, 0.0f,
                                                 0.0f, zHigher,
                                                 xHigher, zHigher,
                                                 0.0f, 0.0f,
                                                 xHigher, zHigher,
                                                 xHigher, 0.0f
                                               }));
    }
    return { vertices, normals, uvs };
}

MeshData GetMeshFromWallCell(const WalledEntry* entry) {
    Byte walls = entry->GetWalls();
    if((walls & (FrontWall | BackWall | LeftWall | RightWall)) == 0) {
        return {TG::Tensor(0, 3, nullptr), TG::Tensor(0, 3, nullptr), TG::Tensor(0, 3, nullptr)};
    }
    float x = entry->GetPosition().x;
    float y = entry->GetPosition().y;
    float z = entry->GetPosition().z;

    TG::Tensor vertices(0, 3, nullptr);
    TG::Tensor normals(0, 3, nullptr);
    TG::Tensor uvs(0, 2, nullptr);
    float horOffset = (rand() % 4) / 4.0f;
    float verOffset = (rand() % 3) / 3.0f;
    if(walls & FrontWall) {
        vertices = TG::CreateTensor(12, 3, {
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
        normals = TG::CreateTensor(12, 3, {
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
        uvs = TG::CreateTensor(12, 2, {
                                   horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                   horOffset + 1.0f / 4, verOffset + 0.5f / 3,
                                   horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                   horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                   horOffset + 1.0f / 4, verOffset + 0.1f / 3,
                                   horOffset + 1.0f / 4, verOffset + 0.5f / 3,

                                    horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                    horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                    horOffset + 1.0f / 4, verOffset + 0.5f / 3,
                                    horOffset + 1.0f / 4, verOffset + 0.1f / 3,
                                    horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                    horOffset + 1.0f / 4, verOffset + 0.5f / 3
                               });

    } else {
        vertices = TG::CreateTensor(6, 3, {
                                        x + 0.4f, y + 0.0f, z + 0.6f,
                                        x + 0.4f, y + 1.0f, z + 0.6f,
                                        x + 0.6f, y + 1.0f, z + 0.6f,

                                        x + 0.4f, y + 0.0f, z + 0.6f,
                                        x + 0.6f, y + 1.0f, z + 0.6f,
                                        x + 0.6f, y + 0.0f, z + 0.6f
                                    });
        normals = TG::CreateTensor(6, 3, {
                                        0.0f, 0.0f, 1.0f,
                                        0.0f, 0.0f, 1.0f,
                                        0.0f, 0.0f, 1.0f,
                                        0.0f, 0.0f, 1.0f,
                                        0.0f, 0.0f, 1.0f,
                                        0.0f, 0.0f, 1.0f
                                   });
        uvs = TG::CreateTensor(6, 2, {
                                    horOffset + 0.0f / 4, verOffset + -0.1f / 3,
                                    horOffset + 1.0f / 4, verOffset + -0.1f / 3,
                                    horOffset + 1.0f / 4, verOffset + 0.1f / 3,
                                    horOffset + 0.0f / 4, verOffset + -0.1f / 3,
                                    horOffset + 1.0f / 4, verOffset + 0.1f / 3,
                                    horOffset + 0.0f / 4, verOffset + 0.1f / 3
                               });
    }
    if(walls & BackWall) {
        vertices = vertices.Add(TG::CreateTensor(12, 3, {
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
                                                }));
        normals = normals.Add(TG::CreateTensor(12, 3, {
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
                                               }));
        uvs = uvs.Add(TG::CreateTensor(12, 2, {
                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.9f / 3,

                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.9f / 3
                               }));
    } else {
        vertices = vertices.Add(TG::CreateTensor(6, 3, {
                                                   x + 0.6f, y + 0.0f, z + 0.4f,
                                                   x + 0.6f, y + 1.0f, z + 0.4f,
                                                   x + 0.4f, y + 0.0f, z + 0.4f,

                                                   x + 0.4f, y + 0.0f, z + 0.4f,
                                                   x + 0.6f, y + 1.0f, z + 0.4f,
                                                   x + 0.4f, y + 1.0f, z + 0.4f,
                                                }));
        normals = normals.Add(TG::CreateTensor(6, 3, {
                                                   0.0f, 0.0f, -1.0f,
                                                   0.0f, 0.0f, -1.0f,
                                                   0.0f, 0.0f, -1.0f,
                                                   0.0f, 0.0f, -1.0f,
                                                   0.0f, 0.0f, -1.0f,
                                                   0.0f, 0.0f, -1.0f
                                               }));
        uvs = uvs.Add(TG::CreateTensor(6, 2, {
                                           horOffset + 0.0f / 4, 0.1f / 3,
                                           horOffset + 1.0f / 4, 0.1f / 3,
                                           horOffset + 0.0f / 4, -0.1f / 3,
                                           horOffset + 0.0f / 4, -0.1f / 3,
                                           horOffset + 1.0f / 4, 0.1f / 3,
                                           horOffset + 1.0f / 4, -0.1f / 3
                               }));
    }
    if(walls & RightWall) {
        vertices = vertices.Add(TG::CreateTensor(12, 3, {
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
                                                }));
        normals = normals.Add(TG::CreateTensor(12, 3, {
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
                                               }));
        uvs = uvs.Add(TG::CreateTensor(12, 2, {
                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.1f / 3,

                                            horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                            horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                            horOffset + 1.0f / 4, verOffset + 0.5f / 3,
                                            horOffset + 1.0f / 4, verOffset + 0.5f / 3,
                                            horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                            horOffset + 1.0f / 4, verOffset + 0.1f / 3
                               }));
    } else {
        vertices = vertices.Add(TG::CreateTensor(6, 3, {
                                                   x + 0.6f, y + 0.0f, z + 0.4f,
                                                   x + 0.6f, y + 0.0f, z + 0.6f,
                                                   x + 0.6f, y + 1.0f, z + 0.6f,

                                                   x + 0.6f, y + 1.0f, z + 0.6f,
                                                   x + 0.6f, y + 1.0f, z + 0.4f,
                                                   x + 0.6f, y + 0.0f, z + 0.4f
                                                }));
        normals = normals.Add(TG::CreateTensor(6, 3, {
                                                   1.0f, 0.0f, 0.0f,
                                                   1.0f, 0.0f, 0.0f,
                                                   1.0f, 0.0f, 0.0f,
                                                   1.0f, 0.0f, 0.0f,
                                                   1.0f, 0.0f, 0.0f,
                                                   1.0f, 0.0f, 0.0f
                                               }));
        uvs = uvs.Add(TG::CreateTensor(6, 2, {
                                           horOffset + 0.0f / 4, verOffset + -0.1f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.1f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.1f / 3,
                                           horOffset + 1.0f / 4, verOffset + -0.1f / 3,
                                           horOffset + 0.0f / 4, verOffset + -0.1f / 3
                               }));
    }
    if(walls & LeftWall) {
        vertices = vertices.Add(TG::CreateTensor(12, 3, {
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
                                                }));
        normals = normals.Add(TG::CreateTensor(12, 3, {
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
                                               }));
        uvs = uvs.Add(TG::CreateTensor(12, 2, {
                                           horOffset + 0.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.5f / 3,

                                           horOffset + 0.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.9f / 3,
                                           horOffset + 0.0f / 4, verOffset + 0.5f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.5f / 3
                               }));
    } else {
        vertices = vertices.Add(TG::CreateTensor(6, 3, {
                                                   x + 0.4f, y + 0.0f, z + 0.6f,
                                                   x + 0.4f, y + 0.0f, z + 0.4f,
                                                   x + 0.4f, y + 1.0f, z + 0.6f,

                                                   x + 0.4f, y + 1.0f, z + 0.6f,
                                                   x + 0.4f, y + 0.0f, z + 0.4f,
                                                   x + 0.4f, y + 1.0f, z + 0.4f
                                                }));
        normals = normals.Add(TG::CreateTensor(6, 3, {
                                                   -1.0f, 0.0f, 0.0f,
                                                   -1.0f, 0.0f, 0.0f,
                                                   -1.0f, 0.0f, 0.0f,
                                                   -1.0f, 0.0f, 0.0f,
                                                   -1.0f, 0.0f, 0.0f,
                                                   -1.0f, 0.0f, 0.0f
                                               }));
        uvs = uvs.Add(TG::CreateTensor(6, 2, {
                                           horOffset + 0.0f / 4, verOffset + 0.1f / 3,
                                           horOffset + 0.0f / 4, verOffset + -0.1f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.1f / 3,
                                           horOffset + 1.0f / 4, verOffset + 0.1f / 3,
                                           horOffset + 0.0f / 4, verOffset + -0.1f / 3,
                                           horOffset + 1.0f / 4, verOffset + -0.1f / 3
                               }));
    }
    return {vertices, normals, uvs};
}

namespace {

void Break(Grid& grid, int i, int j, Byte direction) {
    switch (direction) {
    case LeftWall:
        assert(i > 0);
        grid[i][0][j]->Remove(FrontWall);
        grid[i][0][j+1]->Remove(BackWall);
        break;
    case RightWall:
        assert(i < grid.XLength() - 2);
        grid[i+1][0][j]->Remove(FrontWall);
        grid[i+1][0][j+1]->Remove(BackWall);
        break;
    case BackWall:
        assert(j > 0);
        grid[i+1][0][j]->Remove(LeftWall);
        grid[i][0][j]->Remove(RightWall);
        break;
    case FrontWall:
        assert(j < grid.ZLength() - 2);
        grid[i+1][0][j+1]->Remove(LeftWall);
        grid[i][0][j+1]->Remove(RightWall);
        break;
    default:
        assert(false);
        break;
    }
}

Byte GetAllowed(Grid& grid, int i, int j, std::vector<std::vector<bool>>& accessable) {
    assert(i < grid.XLength() - 1);
    assert(j < grid.ZLength() - 1);
    Byte allowed = 0;
    if(i > 0 && !accessable[i-1][j]) {
        allowed |= LeftWall;
    }
    if(j > 0 && !accessable[i][j-1]) {
        allowed |= BackWall;
    }
    if(i < grid.XLength() - 2 && !accessable[i+1][j]) {
        allowed |= RightWall;
    }
    if(j < grid.ZLength() - 2 && !accessable[i][j+1]) {
        allowed |= FrontWall;
    }
    return allowed;
}

void BreakWalls(Grid& grid, int i, int j, std::vector<std::vector<bool>>& accessable) {
    accessable[i][j] = true;
    while(true) {
        Byte allowed = GetAllowed(grid, i, j, accessable);
        int allowedSize = ((allowed & FrontWall) != 0) + ((allowed & BackWall) != 0) +
                ((allowed & LeftWall) != 0) + ((allowed & RightWall) != 0);
        if(allowedSize == 0) {
            return;
        }
        int dirN = rand() % allowedSize;
        Byte chosen = LeftWall;
        for(int i = 0; i <= dirN; ++i) {
            chosen = NextWall(chosen, allowed);
        }
        int nextI = i;
        int nextJ = j;
        switch (chosen) {
        case LeftWall:
            --nextI;
            break;
        case RightWall:
            ++nextI;
            break;
        case BackWall:
            --nextJ;
            break;
        case FrontWall:
            ++nextJ;
            break;
        default:
            assert(false);
            break;
        }
        Break(grid, i, j, chosen);
        BreakWalls(grid, nextI, nextJ, accessable);
    }
}

void ProcessGrid(Grid& grid) {
    for(int y = 0; y < grid.YLength(); ++y) {
        for(int z = 0; z < grid.ZLength(); ++z) {
            grid[0][y][z]->Remove(LeftWall);
        }
        for(int z = 0; z < grid.ZLength(); ++z) {
            grid[grid.XLength()-1][y][z]->Remove(RightWall);
        }

        for(int x = 0; x < grid.XLength(); ++x) {
            grid[x][y][0]->Remove(BackWall);
        }
        for(int x = 0; x < grid.XLength(); ++x) {
            grid[x][y][grid.ZLength()-1]->Remove(FrontWall);
        }
    }

    std::vector<std::vector<bool>> accessable;
    accessable.resize(grid.XLength() - 1);
    for(int i = 0; i < grid.XLength() - 1; ++i) {
        accessable[i].resize(grid.ZLength() - 1, false);
    }
    BreakWalls(grid, 0, 0, accessable);
}

glm::vec3 ProcessVec(std::ifstream& file) {
    glm::vec3 vec;
    file >> vec[0] >> vec[1] >> vec[2];
    return vec;
}

void CopyVec3(glm::vec3& from, float* to, int& offset) {
     *(to+offset) = from[0];
     offset += 1;
     *(to+offset) = from[2];
     offset += 1;
     *(to+offset) = from[1];
     offset += 1;
}

}

MazeSceneBuilder::MazeSceneBuilder(std::vector<std::unique_ptr<TG::IMaterial>>& materials,
                                   TierGine::IBackend& backend) :
    width(0),
    height(0),
    materials(materials),
    backend(backend),
    wallMaterial(nullptr),
    floorMaterial(nullptr)
{
}

TierGine::SimpleScene* MazeSceneBuilder::CreateSceneAndGrid(TG::IContext& context,
                                          const TG::ICamera& camera,
                                          TG::IPipeline& defaultPipeline)
{
    loadMaterials(context.CreateTextureSampler("diffuseMap"), context);
    std::unique_ptr<TG::SimpleScene> scene(new TG::SimpleScene(camera, defaultPipeline));
    {
        TG::IMesh* mesh = context.CreateMesh();
        TG::Tensor vertices(0, 3, nullptr);
        TG::Tensor normals(0, 3, nullptr);
        TG::Tensor uvs(0, 2, nullptr);
        int n = 42;
        grid.reset(new Grid(n, 1, n));
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                (*grid)[i][0][j].reset(new WalledEntry(i, 0, j, FrontWall | BackWall | LeftWall | RightWall ));
            }
        }
        ProcessGrid(*grid.get());
        for(int i = 0; i < n; ++i) {
            for(int j = 0; j < n; ++j) {
                auto [v, norm, uv] = GetMeshFromWallCell(static_cast<const WalledEntry*>((*grid)[i][0][j].get()));
                vertices = vertices.Add(v);
                normals = normals.Add(norm);
                uvs = uvs.Add(uv);
            }
        }

        std::unique_ptr<TG::BasicModel> basicModel(new TG::BasicModel(*mesh));
        basicModel->SetMaterial(wallMaterial);
        basicModel->LoadFromTensors(vertices, normals, uvs);
        std::unique_ptr<TG::ISceneObject> model(basicModel.release());
        model->SetRenderingMode(TG::RM_FILL, TG::PRS_FRONT);
        scene->Add(model);
    }
    {
        TG::IMesh* mesh = context.CreateMesh();
        TG::Tensor vertices(0, 3, nullptr);
        TG::Tensor normals(0, 3, nullptr);
        TG::Tensor uvs(0, 2, nullptr);

        for(int i = 0; i <= grid->YLength(); ++i) {
            auto [v, norm, uv] = GetMeshFromHorizontalCell(grid->Horizontal(i).get());
            vertices = vertices.Add(v);
            normals = normals.Add(norm);
            uvs = uvs.Add(uv);
        }

        std::unique_ptr<TG::BasicModel> basicModel(new TG::BasicModel(*mesh));
        basicModel->SetMaterial(floorMaterial);
        basicModel->LoadFromTensors(vertices, normals, uvs);
        std::unique_ptr<TG::ISceneObject> model(basicModel.release());
        model->SetRenderingMode(TG::RM_FILL, TG::PRS_FRONT);
        scene->Add(model);
    }
    TG::LightInfo light;
    light.pos = glm::vec3(20.0f, 0.7f, 20.0f);
    light.La = glm::vec3(0.3f, 0.3f, 0.3f);
    light.Ld = glm::vec3(0.6f, 0.6f, 0.6f);
    light.Ls = glm::vec3(0.1f, 0.1f, 0.1f);

    scene->Light().push_back(light);
    scene->Light().push_back(light);


    return scene.release();
}

TierGine::PhysicsWorld* MazeSceneBuilder::CreatePhysicsEngine()
{
    assert(grid.get() != nullptr);
    return new MazePhysicsEngine(std::move(grid));
}

void MazeSceneBuilder::loadMaterials(TG::ITextureSampler* sampler, TG::IContext& context)
{
    materials.push_back(std::move(backend.CreateMaterial(sampler)));
    wallMaterial = materials[0].get();
    auto wallTexture = context.CreateTexture();
    wallTexture->LoadFromPath("res/textures/wall.jpg");
    auto wallNormal = context.CreateTexture();
    wallNormal->LoadFromPath("res/textures/wall_norm.jpg");
    wallMaterial->SetTexture(wallTexture);
    wallMaterial->SetNormalMap(wallNormal);

    materials.push_back(std::move(backend.CreateMaterial(sampler)));
    floorMaterial = materials[1].get();
    auto floorTexture = context.CreateTexture();
    floorTexture->LoadFromPath("res/textures/floor.jpg");
    auto floorNormal = context.CreateTexture();
    floorNormal->LoadFromPath("res/textures/floor_norm.jpg");
    floorMaterial->SetTexture(floorTexture);
    floorMaterial->SetNormalMap(floorNormal);
}
