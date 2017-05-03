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
	void programInit();
	void Load();
	//draws the Terrain on the screen
	void Draw();
	//Reads in and parses the data from an obj file 
	void init(string filename);

	vector <vec4> getNormals();
	
	//Returns ctr_box
	vec4 provideBoxCenter();

	GLfloat provideBoxMax();

	void translateTerrain(vec3 t);

	void scaleTerrain(vec3 s);

	void rotateTerrain(GLfloat xAxis, GLfloat yAxis, GLfloat zAxis);

	void getShaderLocations();

	void transferSettings();

	GLuint vao;
	GLuint program; //Init Shader Program
private:
	int numVertices;
	int numNormals;
	int numIndicies;
	int numTextures;
	int size;
	bool mapText = false;
	vector<vec4> vertices;
	vector<vec4> normals;
	vector<vec2> textures;
	vector<vector<GLfloat>> heights;
	vector<GLuint> indexList;
	GLfloat box_max;
	vec4 ctr_box;

	//Material file data
	Material mat;

	string objFileName;
	GLuint tex, detailTex, detailTex2;

	GLuint iBuffer, buffer;
	int iPrimitiveRestartIndex;

	//Shader locations
	GLuint ModelViewLoc, ProjectionViewLoc;
	GLuint MAmbientLoc, MDiffuseLoc, MSpecularLoc, ShininessLoc, textBoolLoc, texLoc, dTexLoc;
	GLuint deltaTimeLoc;

	//Transformation matrices
	mat4 translate, scale, rotate;

};


