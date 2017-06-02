#pragma once
#include "Angel.h"

using namespace std;

class DirLight
{
public:
	DirLight();
	DirLight(vec4 pos, vec3 a, vec3 d, vec3 s);
	
	void init(GLuint &program, string pos_Loc, string a_Loc, string d_Loc, string s_Loc);
	void transferSettings(GLuint& program);
	void setPosition(vec4 p);
	void translateLight(vec3 t);
	void scaleLight(vec3 s);
	void rotateLight(vec3 r);
	~DirLight();
protected:
	GLuint LDiffuseLoc, LAmbientLoc, LSpecularLoc, 
		LightPositionLoc;

	vec4 lightPosition;
	vec3 lightAmbient, lightDiffuse, lightSpecular;

};

