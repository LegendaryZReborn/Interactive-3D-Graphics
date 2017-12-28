#pragma once
#include "Angel/Angel.h"
#include "Soil/soil.h"
class Skybox
{
	GLuint vao_cube;
	GLuint  sky_mv_loc;
	GLuint cubebuffer;
	GLuint TextureLoc;
	GLuint tex;
	GLuint program_sky;
	int nWidth;
	int nHeight;

public:
	Skybox(void);
	~Skybox(void);
	void Load(void);
	void Draw(void);
	GLuint sendSkyTex();

};
