#pragma once
#include "Angel.h"
#include<vector>

using namespace std;

class GUI
{
public:
	GUI();
	GUI(GLuint texture, GLuint& program);
	void loadGUI(GLuint& program);
	void drawGUI(GLuint& program);
	~GUI();

private:
	GLuint vao;
	GLuint buffer;
	GLuint tex;
	vector<vec4> vertices;
	vector<vec2> textureCoords;
	int width;
	int height;
};

