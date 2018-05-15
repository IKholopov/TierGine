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

#version 400

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

struct LightInfo
{
    vec3 pos;
    vec3 dir;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
    float ang;
};
uniform LightInfo light[2];

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexCoord;
layout(location = 3) in vec3 vertexTangent;
layout(location = 4) in vec3 vertexBitangent;

out mat3 TBN;
out vec4 lightPosCamSpace[2];
out vec4 lightSourceDirCamSpace[2];
out vec4 posCamSpace;
out vec2 texCoord;

void main()
{
    texCoord = vertexTexCoord;
    mat4 modelViewFull = viewMatrix * modelMatrix;
    mat3 modelViewRot = mat3(vec3(modelViewFull[0]), vec3(modelViewFull[1]), vec3(modelViewFull[2]));
    TBN = transpose(mat3(
                            modelViewRot * normalize(vertexTangent),
                            modelViewRot * normalize(vertexBitangent),
                            modelViewRot * normalize(vertexNormal)
                          ));
    posCamSpace = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    for(int i = 0; i < 2; ++i) {
        lightPosCamSpace[i] = viewMatrix * vec4(light[i].pos, 1.0);
        lightSourceDirCamSpace[i] = viewMatrix * vec4(light[i].dir, 0.0);
    }

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}
