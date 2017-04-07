#version 150

in  vec2 TexCoord;
in  vec4 color;
out vec4 fColor;
uniform sampler2D tex;
uniform  bool mapText;

void main() 
{ 
	if (mapText)
		fColor = texture(tex, TexCoord) * color;	
	else
		fColor = color;
	
} 

