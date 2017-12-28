#version 150

in  vec2 TexCoord;
in vec3 position;
in vec3 normal;
out vec4 fColor;
uniform sampler2D tex;
uniform  bool mapText;
uniform float t;
uniform mat4 mV;
uniform mat3 normalMatrix;

//Lighting and Materials
uniform vec3 MAmbient, MDiffuse, MSpecular; // Material settings
uniform float Shininess;

const int MAX_LIGHTS = 8;
uniform int NUM_LIGHTS;
uniform int NUM_PLIGHTS;

struct Light
{
	vec4 LightPosition;

    vec3 LAmbient;
	vec3 LDiffuse;
	vec3 LSpecular;
};

uniform Light lights[MAX_LIGHTS];

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

uniform PointLight pLight[MAX_LIGHTS];

//Prototypes
 vec4 moveLight(vec4 l);

 vec3 calcDirLight(Light light, vec3 pos, vec3 E, vec3 N);

vec3 calcPointLight(PointLight light, vec3 E, vec3 N, vec3 pos);


void main()
{

	vec3 lightCol = vec3(0.0, 0.0, 0.0);

	vec3 E;
	vec3 N;

	E = normalize(-position);

	for (int i = 0; i < NUM_LIGHTS; i++)
	{
		lightCol += calcDirLight(lights[i], position, E, normal);
	}

	for (int i = 0; i < NUM_PLIGHTS; i++)
	{
		lightCol += calcPointLight(pLight[i], E, normal, position);
	}


	if (mapText)
		fColor = texture(tex, TexCoord) * vec4(lightCol, 1.0);
	else
		fColor = vec4(lightCol, 1.0);

}


////////////////////////////////////////////////
//Definitions
////////////////////////////////////////////////
 vec4 moveLight(vec4 l)
 {
	float dx = 10 * cos(t);
	float dy = 10 * sin(t);
	l.x += dx;
	l.y += dy;

	return l;
 }

 vec3 calcDirLight(Light light, vec3 pos, vec3 E, vec3 N)
 {

	float Kd, Ks;
 	vec3 ambient, diffuse, specular, color;
 	vec3 L;
	vec3 H;

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

	color = ambient + diffuse + specular;
	return color;
 }

vec3 calcPointLight(PointLight light, vec3 E, vec3 N, vec3 pos)
{
	float Kd, Ks, distance, attenuation;
 	vec3 ambient, diffuse, specular;
    vec3 ambientGlobal = vec3(0.25, 0.25, 0.25);
	vec3 color;
	vec3 L;
	vec3 H;

    //light.LightPosition = moveLight(vec4(light.LightPosition.xyz - pos, 1.0));
	L = normalize(light.LightPosition.xyz - pos);
	H = normalize (L + E);

	//Compute terms in illumination equation
	ambient = light.LAmbient * MAmbient;

	Kd = max(dot(N, L), 0.0);
	diffuse = Kd*light.LDiffuse*MDiffuse;

	Ks = pow(max(dot(N, H), 0.0), Shininess);
	specular = Ks * light.LSpecular * MSpecular;

	if(dot(N, L) < 0)
	{
		specular = vec3(0.0,0.0,0.0);
	}


	distance = length(light.LightPosition.xyz - pos);
	attenuation =  1.0f/((light.constant + light.linear * distance) + light.quadratic * (distance * distance));
	//attenuation =  1.0f/(1.0 + 0.000000001);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	color = ambient + diffuse + specular;

	return color;
}
