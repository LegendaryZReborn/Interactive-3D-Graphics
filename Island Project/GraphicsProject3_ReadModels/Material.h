#pragma once
#include "Angel.h"
#include <string>

using namespace std;

class Material
{
public:
	Material();
	Material(GLfloat shine, GLfloat dis, GLfloat opD,
		vec3 ambient, vec3 diffuse, vec3 specular, GLuint i, string textureF);
	Material(const Material& mat1);
	~Material();

	//Material file data
	GLfloat Shininess, mDissolve, mOpticalDensity;
	vec3 mAmbient, mDiffuse, mSpecular;
	GLuint illum;
	string textureFile = "";
};

