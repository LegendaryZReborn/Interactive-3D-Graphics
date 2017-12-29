#version 430

in vec4 color;

out vec4 f_color;

layout(location = 1) uniform float time;

void main(){
   vec4 nColor = vec4(color.x + sin(time), color.y + cos(time), color.z + sin(time) + cos(time), 1.0);
   f_color = nColor;
}
