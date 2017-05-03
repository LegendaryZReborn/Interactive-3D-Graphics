#pragma once
#include <string>
#include <vector>
#include "Angel.h"
#include "Material.h"

class Water
{
public:
	Water();
	Water(int size, bool hasTexture, string texFile);
	void initProgram();
	void load();
	void createPlane();
	void draw();
	void getShaderLocations();
	void transferSettings();
	void translateWater(vec3 t);
	void scaleWater(vec3 s);
	void rotateWater(vec3 r);
	void changeProgram(GLuint program);
	GLuint getProgram();
	void setFrequency(float f);
	void setReflectionTex(GLuint ref);
	~Water();

	GLuint vao;
	GLuint program;
private:
	GLuint buffer;
	GLuint iBuffer;
	GLint size;
	vector<vec4> vertices;
	vector<GLuint> indicies;
	vector<vec4> textures;
	vector <vec4> normals;
	mat4 translate, scale, rotate;
	GLuint tex, reflectTex;
	bool hasTexture;
	Material mat;

	//Shader settings
	GLuint ModelViewLoc, ProjectionViewLoc;
	GLuint MAmbientLoc, MDiffuseLoc, MSpecularLoc, ShininessLoc, textBoolLoc, texLoc;
	GLuint deltaTimeLoc, amplitudeLoc, speedLoc, directionLoc, frequencyLoc;

	GLfloat amplitude[8];
	GLfloat speed[8];
	vec2 direction[8];
	GLfloat frequency;

	vec4 ctr_box;
};

