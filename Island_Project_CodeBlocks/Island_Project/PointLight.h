#pragma once
#include "DirLight.h"

class PointLight: public DirLight
{
public:
	PointLight();
	PointLight(vec4 pos, vec3 a, vec3 d, vec3 s, GLfloat c,
		GLfloat l, GLfloat q);
	void init(GLuint &program, string pos_Loc, string a_Loc, string d_Loc, string s_Loc,
		string const_Loc, string lin_Loc, string quad_Loc);
	void transferSettings(GLuint& program);

	~PointLight();

private:
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;
	GLuint constantLoc, quadraticLoc, linearLoc;

};

