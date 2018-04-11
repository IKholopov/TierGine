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

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;

struct LightInfo
{
    vec3 pos;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
};
uniform LightInfo light[2];

struct MaterialInfo
{
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float shininess;
};
uniform MaterialInfo material;

in mat3 TBN;
in vec4 lightPosCamSpace[2];
in vec4 posCamSpace;
in vec2 texCoord;

out vec4 fragColor;

void main()
{
    vec3 color = vec3(0.0f);
    vec3 diffuseColor = texture(diffuseTex, texCoord).rgb;
    for(int i = 0; i < 2; ++i) {
        vec3 dist = lightPosCamSpace[i].xyz - posCamSpace.xyz;
        vec3 lightDirCamSpace = normalize(dist);
        float distL2 = max(length(dist), 1.4);
        vec3 normal = normalize(texture(normalTex, texCoord).rgb * 2.0 - 1.0);

        float NdotL = max(dot(normal, TBN * lightDirCamSpace.xyz), 0.0);


        color += diffuseColor * ( (light[i].La * material.Ka) / pow(distL2,2) + (light[i].Ld * material.Kd * NdotL) / pow(distL2,2) ); //цвет вершины
        if (NdotL > 0.0) {
            vec3 viewDirection = normalize(- posCamSpace.xyz);
            vec3 halfVector = TBN * normalize(lightDirCamSpace.xyz + viewDirection);

            float blinnTerm = max(dot(normal, halfVector), 0.0);
            blinnTerm = pow(blinnTerm, material.shininess);

            color += light[i].Ls * material.Ks * blinnTerm / pow(distL2,2);
        }
    }

    fragColor = vec4(color, 1.0);
}
