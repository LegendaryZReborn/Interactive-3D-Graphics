#version 150

in  vec4 vPosition;
in	vec4 vNormal;
in  vec4 vColor;
in  vec2 texcoord;
out vec2 TexCoord;
out vec4 color;

uniform mat4 model_view;
uniform mat4 proj;
uniform mat4 translate;
uniform mat4 scale;

//Lighting and Materials
uniform vec3 LAmbient, LDiffuse, LSpecular;
uniform vec3 MAmbient, MDiffuse, MSpecular; // Material settings
uniform vec4 LightPosition;

uniform float Shininess;


void main() 
{
    vec3 pos = (model_view * vPosition).xyz;
	vec3 E = normalize(-pos);
	vec3 L;
	vec3 H;
	//Transform vertex normal into eye coordinates
	vec3 N = normalize(model_view * vNormal).xyz;
	vec3 ambient, diffuse, specular;
	vec3 diffuseT= vec3(0.0, 0.0, 0.0), ambientT = vec3(0.0, 0.0, 0.0), specularT = vec3(0.0, 0.0, 0.0);
			float Kd, Ks;


	for(int i = 0; i < 1; i++)
	{
		//for the first position
		L = normalize(LightPosition.xyz - pos);
		H = normalize (L + E);

		//Compute terms in illumination equation
		ambient = LAmbient * MAmbient;	
	
		 Kd = max(dot(L, N), 0.0);
		diffuse = Kd*LDiffuse*MDiffuse;
		Ks = pow(max(dot(N, H), 0.0), Shininess);
		specular = Ks * LSpecular * MSpecular;

		if(dot(L, N) < 0)
		{
			specular = vec3(0.0,0.0,0.0);
		}

		diffuseT += diffuse;
		specularT += specular;
		ambientT += ambient;
	}
   
  // if(diffuseT != 0 && specularT != 0; && ambientT !=0)
  //  diffuseT /= 2;
 //	specularT /= 2;
//	ambientT /= 2;

    gl_Position = proj* model_view* scale * translate *vPosition;
	TexCoord = texcoord;
	color = vec4(diffuseT + ambientT + specularT , 1.0);
		//color = vec4(1.0, 0.0, 0.0, 1.0);

	
} 
