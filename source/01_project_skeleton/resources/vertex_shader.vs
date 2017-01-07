#version 330 core

layout (location = 0) in vec3 vertPos;

void main() {
    // does not alter the verticies at all
    gl_Position = vec4(vertPos, 1);
}