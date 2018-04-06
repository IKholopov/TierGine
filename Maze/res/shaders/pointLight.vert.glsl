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

uniform mat3 normalToCameraMatrix;

struct LightInfo
{
    vec3 pos;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
};
uniform LightInfo light;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;

out vec3 normalCamSpace;
out vec4 lightPosCamSpace;
out vec4 posCamSpace;

void main()
{
    posCamSpace = viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
    normalCamSpace = normalize(normalToCameraMatrix * vertexNormal);
    lightPosCamSpace = viewMatrix * vec4(light.pos, 1.0);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vertexPosition, 1.0);
}