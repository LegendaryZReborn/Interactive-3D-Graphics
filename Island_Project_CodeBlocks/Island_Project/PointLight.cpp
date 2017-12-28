#include "PointLight.h"

PointLight::PointLight() : DirLight()
{

}

PointLight::PointLight(vec4 pos, vec3 a, vec3 d, vec3 s, GLfloat c,
	GLfloat l, GLfloat q): DirLight(pos, a, d, s)
{
	constant = c;
	linear = l;
	quadratic = q;
}

void PointLight::init(GLuint &program, string pos_Loc, string a_Loc, string d_Loc, string s_Loc,
	string const_Loc, string lin_Loc, string quad_Loc)
{
	DirLight::init(program, pos_Loc, a_Loc, d_Loc, s_Loc);

	quadraticLoc = glGetUniformLocation(program, quad_Loc.c_str());

	constantLoc = glGetUniformLocation(program, const_Loc.c_str());

	linearLoc = glGetUniformLocation(program, lin_Loc.c_str());

}

void PointLight::transferSettings(GLuint& program)
{
	DirLight::transferSettings(program);

	glUniform1f(constantLoc, constant);
	glUniform1f(linearLoc, linear);
	glUniform1f(quadraticLoc, quadratic);
}


PointLight::~PointLight()
{
}
