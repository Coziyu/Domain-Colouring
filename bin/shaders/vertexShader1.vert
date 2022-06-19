#version 330 core
layout (location = 0) in vec2 coord2d;
layout (location = 1) in vec3 vecCol;

out vec3 out_vecCol;

void main(){
    out_vecCol = vec3(coord2d, 0.0f);
    gl_Position = vec4(coord2d.x, coord2d.y, 0.0f, 1.0f);
}