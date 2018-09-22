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

in vec2 texCoords;
out vec4 frag_color;

void main() {
     frag_color = vec4(texture(portaledTexture, vec2((texCoords[0] - 0.5) / 2 + 0.5f  , (texCoords[1] - 0.5) / 2 + 0.5f )).rgb, 1.0);
}
