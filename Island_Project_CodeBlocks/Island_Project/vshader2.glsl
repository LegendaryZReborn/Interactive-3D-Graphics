#version 150

in  vec4 vPosition;
in	vec4 vNormal;
in  vec2 texcoord;

out vec2 TexCoord;
out vec3 position;
out vec3 normal;
uniform mat4 model_view;
uniform mat3 normalMatrix;
uniform mat4 proj;

void main()
{
	normal = normalMatrix * vNormal.xyz;
	position = (model_view * vPosition).xyz;
    gl_Position = proj* model_view *vPosition;
	TexCoord = texcoord;


}

