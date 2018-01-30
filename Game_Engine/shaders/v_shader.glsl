#version 430 core

layout(location = 0) in vec3 vPosition;
layout(location = 2) in vec2 textureCoords;

out vec2 texCoords;
out vec4 color;

void main(){
    color = vec4(vPosition.x + 0.5, 20, vPosition.y + 0.5, 1.0);
    gl_Position =  vec4(vPosition, 1.0);
    texCoords = textureCoords;
}
