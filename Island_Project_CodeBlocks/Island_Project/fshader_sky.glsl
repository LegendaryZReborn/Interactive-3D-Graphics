#version 150 core
#extension GL_NV_shadow_samplers_cube : enable

in  vec3 texCoord;
out vec4 color;
uniform samplerCube cube_tex;
void main()
{
    color = textureCube(cube_tex, texCoord); 
}
  