#pragma once

#include <string>
#include <vector>
#include "Angel/Angel.h"
#include "Material.h"
#include <map>
#include <fstream>
#include <iostream>
#include "Object.h"
#include "MultiMeshObj.h"

using namespace std;

class Temple
{
public:
	Temple();
	void load(GLuint program);
	void draw();
	void translateTemple(vec3 t);
	~Temple();


private:
	void readPlacementFile();

	//for pillar data
	map<string, vector<vec4>> locations;
	map<string, vector<vec3>> rotations;
	map<string, vector<vec3>> scales;
	string datPath, fileNamePath;
	vector<string> objNames;
	vector <Object> objects;
	vec3 translate, scale, rotate;
	GLuint program;
};

