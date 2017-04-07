#version 150 core

in  vec3 skyPosition;
uniform mat4 P;
uniform mat4 MV;

out vec3 texCoord;

void main() 
{
   texCoord = skyPosition;
   gl_Position = P*MV*vec4(skyPosition, 1.0);
} 

