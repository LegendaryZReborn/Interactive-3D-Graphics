#version 150

in  vec4 vPosition;
in vec4 vNormal;

out vec4 normal;
out vec3 position;
out vec2 refractionCoeffs;

uniform mat4 model_view;
uniform mat4 proj;
uniform float t;



uniform float amplitude[8];
uniform float f;
uniform float speed[8];
uniform vec2 direction[8];


float wave(int i, float x, float z);

float waveHeight(float x, float z);

float waveX(float x, float z);

float waveZ(float x, float z);

vec2 calcRefractionCoeffs();

float dWavedx(int i, float x, float z);
   
float dWavedy(int i, float x, float z);

vec4 waveNormal(float x, float z);


void main() 
{
	vec3 lightCol =  vec3(0.0, 0.0, 0.0); 
	vec4 v;
	float height;
	vec2 rK;

	//periodically adjust vertex y, x and z
	height = waveHeight(vPosition.x, vPosition.z);
	v.x += waveX(v.x, v.z);
	v.z += waveZ(v.x, v.z);
	v = vPosition;
	v.y = height;

	//calculate the Fresnel coefficients
	rK = calcRefractionCoeffs();

	position = (model_view * v).xyz;
    gl_Position = proj* model_view *v;
	refractionCoeffs = rK;



} 

 float wave(int i, float x, float z)
 {
 	//float f = (2 * 3.14159)/waveL[i];
	float P = (speed[i] * f);
	float theta = dot(direction[i], vec2(x, z));
	return amplitude[i] * sin(theta * f  + t * P);

 }

 float waveHeight(float x, float z)
 {
	float height = 0.0;
	for(int i = 0; i < 8; ++i)
	{
		height += wave(i, x, z);
	}

	return height;
 }

 float waveX(float x, float z)
 {
	float eX = 0.0;
	float phase; 
	float theta;
	float A ;

	for (int i = 0; i < 8; ++i)
	{
		phase = speed[i] * f;
		theta = dot(direction[i], vec2(x, z));
		A = amplitude[i] * direction[i].x * f;
		eX += A * cos(theta * f + t/1.5 * phase);
	}

	return eX;
 }

 float waveZ(float x, float z)
 {
	float zEE = 0.0;
	float phase;
    float theta;
    float A;

	for (int i = 0; i < 8; ++i)
	{
		phase = speed[i] * f;
		theta = dot(direction[i], vec2(x, z));
		A = amplitude[i] * direction[i].y * f;
		zEE += A * cos(theta * f + t*1.5 * phase);
	}
	return zEE;
 }

 //calculates the Fresnel coefficients
 vec2 calcRefractionCoeffs()
 {
 
  	float Rs, Rp, incidentAngle, refractAngle;

	vec3 pos = (model_view * vPosition).xyz;
	vec3 E = normalize(-pos);
	//Transform vertex normal into eye coordinates
	vec4 n = waveNormal(pos.x, pos.z);
	vec3 N = normalize(model_view * n).xyz;
	normal = vec4(N, 0.0);

	//calculate the incident and refract angles
	incidentAngle = acos(dot(E, N));
	refractAngle = asin(sin(incidentAngle)/1.333f);

	//calculate the Frsnel coefficients for both polarization orientations
	Rs = pow( (sin(incidentAngle - refractAngle) / sin(incidentAngle + refractAngle)), 2.0f);
	Rp = pow( (1.333f*cos(incidentAngle) - cos(refractAngle) ) / 
	(1.33f*cos(incidentAngle) + cos(refractAngle) ), 2.0f );

	return vec2(Rs, Rp);

 }

 float dWavedx(int i, float x, float z) {
    float phase = speed[i] * f;
    float theta = dot(direction[i], vec2(x, z));
    float A = amplitude[i] * direction[i].x * f;
    return A * cos(theta * f + t * phase);
}

float dWavedy(int i, float x, float z) {
    float phase = speed[i] * f;
    float theta = dot(direction[i], vec2(x, z));
    float A = amplitude[i] * direction[i].y * f;
    return A * cos(theta * f + t * phase);
}

vec4 waveNormal(float x, float z) {
    float dx = 0.0;
    float dz = 0.0;
    for (int i = 0; i < 8; ++i) {
        dx += dWavedx(i, x, z);
        dz += dWavedy(i, x, z);
    }
    vec4 n = vec4(-dx, 1.0, -dz, 0.0);
    return normalize(n);
}