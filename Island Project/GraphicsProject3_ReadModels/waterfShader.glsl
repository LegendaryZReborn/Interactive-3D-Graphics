
#version 150

in vec4 normal;
in vec3 position;
in vec2 refractionCoeffs;

out vec4 fColor;
uniform samplerCube reflectTex;

//Lighting and Materials
uniform vec3 MAmbient, MDiffuse, MSpecular; // Material settings
uniform float Shininess;

struct Light
{
	vec4 LightPosition; 
    vec3 LAmbient;
	vec3 LDiffuse; 
	vec3 LSpecular;
};

#define NUM_LIGHTS 1  
 uniform Light lights[NUM_LIGHTS];


 vec3 calcDirLight(Light light);

void main() 
{ 
	vec3 lightCol = vec3(0.0, 0.0, 0.0);

		for(int i = 0; i < NUM_LIGHTS; i++)
		{
			lightCol += calcDirLight(lights[i]);
		}

	vec3 I = normalize(-position);
	vec3 R = reflect(I, normalize(normal.xyz));

	fColor = vec4(lightCol, (refractionCoeffs.x + refractionCoeffs.y)/2.0) * (0.3 * texture(reflectTex, R));
	
} 


  vec3 calcDirLight(Light light)
 {

	float Kd, Ks;
 	vec3 ambient, diffuse, specular;

	vec3 E = normalize(-position);
	//Transform vertex normal into eye coordinates
	vec3 N = normal.xyz;
	vec3 L = normalize(light.LightPosition.xyz - position);
	vec3 H = normalize (L + E);

	//Compute terms in illumination equation
	ambient = light.LAmbient * MAmbient;	
	
	Kd = max(dot(L, N), 0.0);
	diffuse = Kd*light.LDiffuse*MDiffuse;

	Ks = pow(max(dot(N, H), 0.0), Shininess);
	specular = Ks * light.LSpecular * MSpecular;

	if(dot(L, N) < 0)
	{
		specular = vec3(0.0,0.0,0.0);
	}

	return (ambient + diffuse + specular);
 }
