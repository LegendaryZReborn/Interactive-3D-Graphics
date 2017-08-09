#pragma once
#include "Angel.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)


class Cube
{
public:
	Cube();
	void quad(int a, int b, int c, int d);
	//draws cube with specified colors
	void colorcube();
	void Load(GLuint &program);
	~Cube();

private:
	point4 points[NumVertices];
	color4 colors[NumVertices];

	point4 vertices[8];
	color4 vertex_colors[8];

	// Vertices of a unit cube centered at origin, sides aligned with axes
	//point4 vertices[8] = {
	//	point4(-0.5, -0.5, 0.5, 1.0),
	//	point4(-0.5, 0.5, 0.5, 1.0),
	//	point4(0.5, 0.5, 0.5, 1.0),
	//	point4(0.5, -0.5, 0.5, 1.0),
	//	point4(-0.5, -0.5, -0.5, 1.0),
	//	point4(-0.5, 0.5, -0.5, 1.0),
	//	point4(0.5, 0.5, -0.5, 1.0),
	//	point4(0.5, -0.5, -0.5, 1.0)
	//};

	//// RGBA olors
	//color4 vertex_colors[8] = {
	//	color4(0.0, 0.0, 0.0, 1.0),  // black
	//	color4(1.0, 0.0, 0.0, 1.0),  // red
	//	color4(1.0, 1.0, 0.0, 1.0),  // yellow
	//	color4(0.0, 1.0, 0.0, 1.0),  // green
	//	color4(0.0, 0.0, 1.0, 1.0),  // blue
	//	color4(1.0, 0.0, 1.0, 1.0),  // magenta
	//	color4(1.0, 1.0, 1.0, 1.0),  // white
	//	color4(0.0, 1.0, 1.0, 1.0)   // cyan
	//};

	//----------------------------------------------------------------------------

	// quad generates two triangles for each face and assigns colors
	//    to the vertices
	int Index = 0;

	
};

