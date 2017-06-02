#version 150

in  vec4 vPosition;
in	vec4 vNormal;
in  vec4 vColor;
in  vec2 texcoord;
out vec2 TexCoord;
out vec4 Norm;
out vec4 color;

uniform mat4 model_view;
uniform mat4 proj;
uniform float t;

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

//Light source with attenuation applied during calculations
struct PointLight
{
	vec4 LightPosition; 

    vec3 LAmbient;
	vec3 LDiffuse; 
	vec3 LSpecular;

	float constant;
    float linear;
    float quadratic; 
};

uniform PointLight pLight;

//Prototypes
 vec4 moveLight(vec4 l);

 vec3 calcDirLight(Light light);

vec3 calcPointLight(PointLight light, vec3 E, vec3 N, vec3 pos);



void main() 
{
	vec3 lightCol = vec3(0.0, 0.0, 0.0); 

	vec3 pos; 
	vec3 E;
	vec3 N;

	pos = (model_view * vPosition).xyz;
	E = normalize(-pos);
	N = normalize(model_view * vNormal).xyz;

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		lightCol += calcDirLight(lights[i]);
	}

	
	//moveLight(pLight.LightPosition);
	//lightCol += calcPointLight(pLight, E, N, pos);

	Norm = vNormal;
    gl_Position = proj* model_view *vPosition;
	TexCoord = texcoord;
	color =  vec4(lightCol , 1.0);

} 


////////////////////////////////////////////////
//Definitions
////////////////////////////////////////////////
 vec4 moveLight(vec4 l)
 {
	float dx = 10 * cos(t);
	float dy = 10 * sin(t );
	l.x += dx;
	l.y += dy;

	return l;
 }

 vec3 calcDirLight(Light light)
 {

	float Kd, Ks;
 	vec3 ambient, diffuse, specular;

	vec3 pos = (model_view * vPosition).xyz;
	vec3 E = normalize(-pos);
	//Transform vertex normal into eye coordinates
	vec3 N = normalize(model_view * vNormal).xyz;
	vec3 L = normalize(light.LightPosition.xyz - pos);
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

vec3 calcPointLight(PointLight light, vec3 E, vec3 N, vec3 pos)
{	
	float Kd, Ks, distance, attenuation;
 	vec3 ambient, diffuse, specular;
	vec3 L; 
	vec3 H;

	light.LightPosition = model_view * light.LightPosition;
	light.LightPosition = moveLight(light.LightPosition);

	L = normalize(light.LightPosition.xyz - pos);
	H = normalize (L + E);

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

	distance = length(light.LightPosition.xyz - pos);
	attenuation =  1.0f/(light.constant + light.linear * distance +
						light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}