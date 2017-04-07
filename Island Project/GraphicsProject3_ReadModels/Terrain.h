//Cavaughn Browne
#pragma once
#include <string>
#include <vector>
#include "Angel.h"
#include "Material.h"


using namespace std;
class Terrain
{
public:
	Terrain();
	Terrain(string filename, bool texture);
	Terrain(string filename, bool texture, Material mat, int size);

	~Terrain();
	void Load(GLuint &program);
	//draws the Terrain on the screen
	void Draw(GLuint& program);
	//Reads in and parses the data from an obj file 
	void init(string filename);

	vector<GLfloat> getHeigtMap();

	vector <vec4> getNormals();
	
	//Returns ctr_box
	vec4 provideBoxCenter();

	GLfloat provideBoxMax();

	void translateTerrain(vec3 t);

	void scaleTerrain(vec3 s);

	void rotateTerrain(GLfloat xAxis, GLfloat yAxis, GLfloat zAxis);

	void getShaderLocations(GLuint& program);

	void transferSettings();

	GLuint vao;
private:
	int numVertices;
	int numNormals;
	int numIndicies;
	int numTextures;
	int size;
	bool mapText = false;
	vector<vec4> mappedVertices;
	vector<vec4> mappedNormalsList;
	vector<vec2> mappedTextures;
	vector<vector<GLfloat>> heights;
	vector<GLfloat> heights2;
	vector<GLuint> indexList;
	GLfloat box_max;
	vec4 ctr_box;

	//Material file data
	Material mat;

	string objFileName;
	GLuint tex;

	GLuint iBuffer, buffer;
	int iPrimitiveRestartIndex;

	//Shader locations
	GLuint ModelViewLoc;
	GLuint MAmbientLoc, MDiffuseLoc, MSpecularLoc, ShininessLoc, textBoolLoc, texLoc;

	//Transformation matrices
	mat4 translate, scale, rotate;

};


