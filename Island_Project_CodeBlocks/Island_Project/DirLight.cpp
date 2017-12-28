#include "DirLight.h"
#include <iostream>
extern mat4 model_view, proj;

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
    vec4 lightPos2 = model_view * translate * scale * rotate * lightPosition;

	glUniform3fv(LAmbientLoc, 1, lightAmbient);
	glUniform3fv(LDiffuseLoc, 1, lightDiffuse);
	glUniform3fv(LSpecularLoc, 1, lightSpecular);
	glUniform4fv(LightPositionLoc, 1, lightPos2);
}

void DirLight::setPosition(vec4 p)
{
	lightPosition = p;
}

vec4 DirLight::getPosition()
{

    return lightPosition;
}

void DirLight::translateLight(vec3 t)
{
    translate = Translate(t);
}

void DirLight::scaleLight(vec3 s)
{
    scale = Scale(s);


}

void DirLight::rotateLight(GLfloat xAxis, GLfloat yAxis, GLfloat zAxis)
{
	rotate = RotateX(xAxis) * RotateY(yAxis) * RotateZ(zAxis);
}

DirLight::~DirLight()
{
}
