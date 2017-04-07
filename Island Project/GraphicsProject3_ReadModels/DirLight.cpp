#include "DirLight.h"



DirLight::DirLight()
{
}

DirLight::DirLight(vec4 pos, vec3 a, vec3 d, vec3 s)
{
	lightPosition = pos;
	lightAmbient = a;
	lightDiffuse = d;
	lightSpecular = s;
}

void DirLight::init(GLuint &program, string pos_Loc, string a_Loc, string d_Loc, string s_Loc)
{
	glUseProgram(program);

	LightPositionLoc = glGetUniformLocation(program, pos_Loc.c_str());
	LAmbientLoc = glGetUniformLocation(program, a_Loc.c_str());
	LDiffuseLoc = glGetUniformLocation(program, d_Loc.c_str());
	LSpecularLoc = glGetUniformLocation(program, s_Loc.c_str());
}

void DirLight::transferSettings(GLuint& program)

{
	glUseProgram(program);

	//send over light settings to the shader
	glUniform3fv(LAmbientLoc, 1, lightAmbient);
	glUniform3fv(LDiffuseLoc, 1, lightDiffuse);
	glUniform3fv(LSpecularLoc, 1, lightSpecular);
	glUniform4fv(LightPositionLoc, 1, lightPosition);
}

DirLight::~DirLight()
{
}
