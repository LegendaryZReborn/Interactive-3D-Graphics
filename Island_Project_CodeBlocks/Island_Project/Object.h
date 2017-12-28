//Cavaughn Browne
#pragma once
#include<string>
#include<vector>
#include "Angel.h"
#include "Material.h"
#include <algorithm>



using namespace std;
class Object
{
public:

	Object(string filename, int numV, int numNorm, int numInd, int numText, bool texture, vector<vec4>& mVerts,
		vector<vec4>& mNormL, vector<vec2>& mTex, GLfloat bMax, vec4 ctrBox, Material mat1);
	Object(string filename);
	~Object();

	//Loads object data onto the gpu
	void Load(GLuint program);
	//Draw the object
	void Draw();
	//Reads in and parses the data from an obj file
	void readData(string filename);
	void readMaterialData(string filename);
	vector<vec4> getVertices();
	vec4 provideBoxCenter();
	GLfloat provideBoxMax();
	//Returns the lowest x, y, and z values of the object
	vec4 provideMinPoint();
    //Returns the highest x, y and z values of the object
	vec4 provideMaxPoint();
	//Returns the points for the
	//bounding box surrounding the object
	vector<vec4> provideBoundingBox();
	//Translates the object by t by modifying the translate matrix
	void translateObj(vec3 t);
	//Translates the object by s by modifying the scale matrix
	void scaleObj(vec3 s);
    //Translates the object by s by modifying the rotate matrix
	void rotateObj(GLfloat xAxis, GLfloat yAxis, GLfloat zAxis);

	GLuint vao;
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
	vec4 boxMinP;
	vec4 boxMaxP;
	vec4 ctr_box;
	vector<vec4> bBoxPoints;
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

	//Transfers data to the shaders
    void getShaderLocations();
	void transferMatSettings();
	void transferMVPSettings();



};

