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

uniform float mult;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;

out vec4 color;

void main() {
     color = (0.5*sin(mult))*vec4(vc, 1.0) + 0.8*vec4(abs(vc), 1.0);
     gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
}
