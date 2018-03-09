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
#include <Primitives.h>
#include <Context.h>

namespace TierGine {

IMesh* Primitives::CreateCubeMesh(TierGine::IContext& context, float size)
{
    IMesh* mesh = context.CreateMesh();
    Tensor vertices = CreateTensor(3*2*6, 3, {
                                       //left
                                       0.0f, 0.0f, 0.0f,
                                       size, 0.0f, 0.0f,
                                       0.0f, size, 0.0f,
                                       size, 0.0f, 0.0f,
                                       size, size, 0.0f,
                                       0.0f, size, 0.0f,
                                       //right
                                       0.0f, 0.0f, size,
                                       0.0f, size, size,
                                       size, 0.0f, size,
                                       size, size, size,
                                       size, 0.0f, size,
                                       0.0f, size, size,
                                       //back
                                       0.0f, 0.0f, 0.0f,
                                       0.0f, size, 0.0f,
                                       0.0f, size, size,
                                       0.0f, 0.0f, size,
                                       0.0f, 0.0f, 0.0f,
                                       0.0f, size, size,
                                       //front
                                       size, 0.0f, 0.0f,
                                       size, size, size,
                                       size, size, 0.0f,
                                       size, 0.0f, 0.0f,
                                       size, 0.0f, size,
                                       size, size, size,
                                       //bottom
                                       0.0f, 0.0f, 0.0f,
                                       0.0f, 0.0f, size,
                                       size, 0.0f, size,
                                       size, 0.0f, 0.0f,
                                       0.0f, 0.0f, 0.0f,
                                       size, 0.0f, size,
                                       //top
                                       0.0f, size,  0.0f,
                                       size, size, size,
                                       0.0f, size, size,
                                       size, size, 0.0f,
                                       size, size, size,
                                       0.0f, size, 0.0f,
                                    });
    Tensor normals = CreateTensor(3*2*6, 3, {
                                       //left
                                       0.0f, 0.0f, -1.0f,
                                       0.0f, 0.0f, -1.0f,
                                       0.0f, 0.0f, -1.0f,
                                       0.0f, 0.0f, -1.0f,
                                       0.0f, 0.0f, -1.0f,
                                       0.0f, 0.0f, -1.0f,
                                       //right
                                       0.0f, 0.0f, 1.0f,
                                       0.0f, 0.0f, 1.0f,
                                       0.0f, 0.0f, 1.0f,
                                       0.0f, 0.0f, 1.0f,
                                       0.0f, 0.0f, 1.0f,
                                       0.0f, 0.0f, 1.0f,
                                       //back
                                       -1.0f, 0.0f, 0.0f,
                                       -1.0f, 0.0f, 0.0f,
                                       -1.0f, 0.0f, 0.0f,
                                       -1.0f, 0.0f, 0.0f,
                                       -1.0f, 0.0f, 0.0f,
                                       -1.0f, 0.0f, 0.0f,
                                       //front
                                       1.0f, 0.0f, 0.0f,
                                       1.0f, 0.0f, 0.0f,
                                       1.0f, 0.0f, 0.0f,
                                       1.0f, 0.0f, 0.0f,
                                       1.0f, 0.0f, 0.0f,
                                       1.0f, 0.0f, 0.0f,
                                       //bottom
                                       0.0f, -1.0f, 0.0f,
                                       0.0f, -1.0f, 0.0f,
                                       0.0f, -1.0f, 0.0f,
                                       0.0f, -1.0f, 0.0f,
                                       0.0f, -1.0f, 0.0f,
                                       0.0f, -1.0f, 0.0f,
                                       //top
                                       0.0f, 1.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f,
                                       0.0f, 1.0f, 0.0f,
                                    });
    mesh->AddAtribute(0, vertices);
    mesh->AddAtribute(1, normals);
    return mesh;
}

}
