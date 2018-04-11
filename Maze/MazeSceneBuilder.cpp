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
#include <TBN.h>
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
                                    0.0f , -0.1f ,
                                    1.0f , -0.1f ,
                                    1.0f , 0.1f ,
                                    0.0f , -0.1f ,
                                    1.0f , 0.1f ,
                                    0.0f , 0.1f
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
                                           0.0f , 0.1f ,
                                           1.0f , 0.1f ,
                                           0.0f , -0.1f ,
                                           0.0f , -0.1f ,
                                           1.0f , 0.1f ,
                                           1.0f , -0.1f
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
                                           0.0f , -0.1f ,
                                           0.0f , 0.1f ,
                                           1.0f , 0.1f ,
                                           1.0f , 0.1f ,
                                           1.0f , -0.1f ,
                                           0.0f , -0.1f
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
                                           0.0f , 0.1f ,
                                           0.0f , -0.1f ,
                                           1.0f , 0.1f ,
                                           1.0f , 0.1f ,
                                           0.0f , -0.1f ,
                                           1.0f , -0.1f
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
                                   const std::vector<MaterialFiles>& materialFiles,
                                   TierGine::IBackend& backend) :
    width(0),
    height(0),
    materials(materials),
    materialFiles(materialFiles),
    backend(backend),
    floorMaterial(nullptr)
{
}

TierGine::SimpleScene* MazeSceneBuilder::CreateSceneAndGrid(TG::IContext& context,
                                          const TG::ICamera& camera,
                                          TG::IPipeline& defaultPipeline)
{
    loadMaterials(context.CreateTextureSampler("diffuseMap"), context);
    std::vector<MeshData> meshes;
    for(int i = 0; i < wallMaterials.size(); ++i) {
        meshes.push_back({
                             TG::Tensor(0, 3, nullptr),
                             TG::Tensor(0, 3, nullptr),
                             TG::Tensor(0, 2, nullptr)
                         });
    }
    std::unique_ptr<TG::SimpleScene> scene(new TG::SimpleScene(camera, defaultPipeline));
    {
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
                int meshIndex = rand() % meshes.size();
                meshes[meshIndex].v = meshes[meshIndex].v.Add(v);
                meshes[meshIndex].norm = meshes[meshIndex].norm.Add(norm);
                meshes[meshIndex].uv = meshes[meshIndex].uv.Add(uv);
            }
        }
        for(int i = 0; i < wallMaterials.size(); ++i) {
            TG::IMesh* mesh = context.CreateMesh();
            std::unique_ptr<TG::BasicModel> basicModel(new TG::BasicModel(*mesh));
            basicModel->SetMaterial(wallMaterials[i]);
            basicModel->LoadFromTensors(meshes[i].v, meshes[i].norm, meshes[i].uv);
            auto [tan, bitan] = TG::TBN::CalculateTangentAndBitangent(meshes[i].v, meshes[i].uv);
            basicModel->AddTangentTensors(tan, bitan);
            std::unique_ptr<TG::ISceneObject> model(basicModel.release());
            model->SetRenderingMode(TG::RM_FILL, TG::PRS_FRONT);
            scene->Add(model);
        }
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
        auto [tan, bitan] = TG::TBN::CalculateTangentAndBitangent(vertices, uvs);
        std::unique_ptr<TG::BasicModel> basicModel(new TG::BasicModel(*mesh));
        basicModel->SetMaterial(floorMaterial);
        basicModel->LoadFromTensors(vertices, normals, uvs);
        basicModel->AddTangentTensors(tan, bitan);
        std::unique_ptr<TG::ISceneObject> model(basicModel.release());
        model->SetRenderingMode(TG::RM_FILL, TG::PRS_FRONT);
        scene->Add(model);
    }
    TG::LightInfo light;
    light.pos = glm::vec3(20.0f, 0.7f, 20.0f);
    light.La = glm::vec3(0.5f, 0.5f, 0.5f);
    light.Ld = glm::vec3(0.5f, 0.5f, 0.5f);
    light.Ls = glm::vec3(0.5f, 0.5f, 0.5f);

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
    floorMaterial = materials[0].get();
    auto floorTexture = context.CreateTexture();
    floorTexture->LoadFromPath("res/textures/floor.jpg");
    textures.insert({"res/textures/floor.jpg", floorTexture});
    auto floorNormal = context.CreateTexture();
    floorNormal->LoadFromPath("res/textures/floor_norm.jpg");
    textures.insert({"res/textures/floor_norm.jpg", floorNormal});
    floorMaterial->SetTexture(floorTexture);
    floorMaterial->SetNormalMap(floorNormal);

    for(auto file = materialFiles.begin(); file != materialFiles.end(); ++file) {
        loadMaterial(sampler, context, *file);
    }
}

void MazeSceneBuilder::loadMaterial(TierGine::ITextureSampler* sampler, TierGine::IContext& context,
                                    const MazeSceneBuilder::MaterialFiles& material)
{

    materials.push_back(std::move(backend.CreateMaterial(sampler)));
    TG::IMaterial* wallMaterial = materials.rbegin()->get();
    TG::ITexture* wallTexture;
    auto found = textures.find(material.TextureName);
    if(found != textures.end()) {
        wallTexture = found->second;
    } else {
        wallTexture = context.CreateTexture();
        wallTexture->LoadFromPath(material.TextureName);
    }
    TG::ITexture* wallNormal;
    found = textures.find(material.NormalMapName);
    if(found != textures.end()) {
        wallNormal = found->second;
    } else {
        wallNormal = context.CreateTexture();
        wallNormal->LoadFromPath(material.NormalMapName);
    }
    wallMaterial->SetTexture(wallTexture);
    wallMaterial->SetNormalMap(wallNormal);
    wallMaterial->GetMaterialInfo().shininess = 128.0f;
    wallMaterials.push_back(wallMaterial);
}
