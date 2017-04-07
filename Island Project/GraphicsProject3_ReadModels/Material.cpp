#include "Material.h"


Material::Material()
{
}

Material::Material(GLfloat shine, GLfloat dis, GLfloat opD,
	vec3 ambient, vec3 diffuse, vec3 specular, GLuint i, string textureF)
{
	Shininess = shine; 
	mDissolve = dis;
	mOpticalDensity = opD;
	mAmbient = ambient;
	mDiffuse = diffuse;
	mSpecular = specular;
	illum = i;
	textureFile = textureF;
}

Material::Material(const Material& mat1)
{
	Shininess = mat1.Shininess;
	mDissolve = mat1.mDissolve;
	mOpticalDensity = mat1.mOpticalDensity;
	mAmbient = mat1.mAmbient;
	mDiffuse = mat1.mDiffuse;
	mSpecular = mat1.mSpecular;
	illum = mat1.illum;
	textureFile = mat1.textureFile;
}

Material::~Material()
{
}
