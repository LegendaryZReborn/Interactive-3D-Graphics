//Cavaughn Browne
#pragma once
#include<string>
#include<vector>
#include "Angel.h"
#include "Material.h"



using namespace std;
class Object 
{
public:

	Object();
	Object(string filename, int numV, int numNorm, int numInd, int numText, bool texture, vector<vec4>& mVerts,
		vector<vec4>& mNormL, vector<vec2>& mTex, GLfloat bMax, vec4 ctrBox, Material mat1);
	Object(string filename);
	~Object();
	void Load();
	void Init();
	//draws the object on the screen
	void Draw();
	//Reads in and parses the data from an obj file 
	void readData(string filename);
	void readMaterialData(string filename);
	vector<vec4> getVertices();
	//Returns ctr_box
	vec4 provideBoxCenter();
	GLfloat provideBoxMax();
	void getShaderLocations();
	void transferSettings();
	void translateObj(vec3 t);
	void scaleObj(vec3 s);
	void rotateObj(GLfloat xAxis, GLfloat yAxis, GLfloat zAxis);

	GLuint vao;
	//Init Shader Program
	GLuint program;
	string objFileName;
private:
	int numVertices;
	int numNormals;
	int numIndicies;
	int numTextures;
	bool mapText = false;
	vector<vec4> mappedVertices;
	vector<vec4> mappedNormalsList;
	vector<vec2> mappedTextures;
	GLfloat box_max;
	vec4 ctr_box;

	GLuint buffer;

	//Material file data
	Material mat;


	//Shader file locations
	GLuint MAmbientLoc, MDiffuseLoc, MSpecularLoc, 
		ShininessLoc, textBoolLoc, texLoc, ModelViewLoc,
		deltaTimeLoc, ProjectionViewLoc;

	//transforms
	mat4 translate, scale, rotate;

	
	GLuint tex;
	



};

