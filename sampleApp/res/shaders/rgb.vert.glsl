#version 400

layout(location = 0) in vec3 vp;
layout(location = 1) in vec4 vc;

out vec4 color;

void main() {
     color = vc;
     gl_Position = vec4(vp, 1.0);
}
