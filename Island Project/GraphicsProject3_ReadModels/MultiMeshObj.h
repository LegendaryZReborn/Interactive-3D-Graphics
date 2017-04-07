#pragma once
#include <string>
#include <vector>
#include "Object.h"
#include "Angel.h"
#include "Material.h"
#include <map>


using namespace std;

class MultiMeshObj 
{
public:
	MultiMeshObj();
	MultiMeshObj::MultiMeshObj(string filename);
	~MultiMeshObj();

	void readData(string filename);

	void readMaterialData(string filename);

	void Load(GLuint &program);
	//draws the object on the screen
	void Draw(GLuint& program);

	vec4 provideBoxCenter();
	GLfloat provideBoxMax();
	void calculateMeshDimensions(GLfloat xmin, GLfloat xmax, GLfloat zmin,
		GLfloat zmax, GLfloat ymin, GLfloat ymax);
	string getFileName();
	void translateObj(vec3 t);


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
	//GLuint *indicies;
	GLuint buffer;

	//Material file data
	
	string objFileName;
	GLuint tex;
	vector<Object> subMeshes;
	map<string, Material> materials;

};

