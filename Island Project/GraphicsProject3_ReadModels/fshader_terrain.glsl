#version 150

in  vec2 TexCoord;
in  vec4 color;
in vec4 Norm;
out vec4 fColor;
uniform sampler2D tex;
uniform sampler2D detailTex;
uniform sampler2D detailTex2;
uniform  bool mapText;

void main() 
{ 
	if (mapText)
	{
		vec4 up = vec4(0.0, 1.0, 0.0, 0.0);
		float angle = dot(normalize(Norm), normalize(up));
		
		fColor = ((1.0 - angle) * texture(detailTex, 40*TexCoord) + angle * texture(detailTex2, 60*TexCoord)) * color;
		
		
		
	}
	else
		fColor = color;
	
} 

