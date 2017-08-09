#version 150

in  vec2 TexCoord;
in  vec4 color;
in vec4 Norm;
in vec4 up;
out vec4 fColor;
uniform sampler2D tex;
uniform sampler2D detailTex;
uniform sampler2D detailTex2;
uniform  bool mapText;

void main()
{
	if (mapText)
	{
		float angle = dot(normalize(Norm), normalize(up));

		fColor = ((1.0 - angle) * texture(detailTex, 100*TexCoord) + angle * texture(detailTex2, 100*TexCoord)) * color;



	}
	else
		fColor = color;

}

