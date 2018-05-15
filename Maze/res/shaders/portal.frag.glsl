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

uniform sampler2D portaledTexture;

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
uniform vec3 portalColor;

in mat3 TBN;
in vec4 lightPosCamSpace[2];
in vec4 lightSourceDirCamSpace[2];
in vec4 posCamSpace;
in vec2 texCoord;

out vec4 fragColor;

float radius() {
    float y =(texCoord[1]-0.5f) * (texCoord[1]-0.5f)/10;
    //y = min((texCoord[1]-0.75f) * (texCoord[1]-0.75f)/1.1, y);
    return (texCoord[0]-0.5f)* (texCoord[0]-0.5f) / 6 + y;
}

void main()
{
    vec3 color = vec3(0.0f);
    vec3 diffuseColor = portalColor;
    float alpha = 0.0f;
    if( radius() < 0.02f )
    {
        alpha = 1.0f;
    }
    if( radius() > 0.018f ) {
        diffuseColor = portalColor;
    } else {
        diffuseColor = texture(portaledTexture, texCoord).rgb;
    }
    fragColor = vec4(diffuseColor, alpha);//vec4(portalColor, alpha);
}
