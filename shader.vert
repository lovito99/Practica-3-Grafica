#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
uniform mat4 projection;

out vec3 vertexColor;

void main() {
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    gl_PointSize = 4.0;
    vertexColor = aColor;
}