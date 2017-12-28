#pragma once
#include <iostream>
#include "Angel/Angel.h"


using namespace std;

class WaterFrameBuffers
{
public:
	WaterFrameBuffers();

	//call when closing the game
	void cleanUp();

	 GLuint getReflectionTexture();//get the resulting texture

	GLuint getRefractionTexture(); //get the resulting texture

	GLuint getRefractionDepthTexture();//get the resulting depth texture

	GLuint createFrameBuffer();

	GLuint createTextureAttachment(int width, int height);

	GLuint createDepthTextureAttachment(int width, int height);

	GLuint createDepthBufferAttachment(int width, int height);

	void bindFrameBuffer(GLuint frameBuffer, int width, int height);

	void bindReflectionFrameBuffer();

	void bindRefractionFrameBuffer();

	void unbindCurrentFrameBuffer();

	void initialiseReflectionFrameBuffer();

	void initialiseRefractionFrameBuffer();

	~WaterFrameBuffers();

private:
	GLuint reflectionFrameBuffer;
	GLuint reflectionTexture;
	GLuint reflectionDepthBuffer;

	GLuint refractionFrameBuffer;
	GLuint refractionTexture;
	GLuint refractionDepthTexture;

	static const int REFLECTION_WIDTH = 320;
	static const int REFLECTION_HEIGHT = 180;

	static const int REFRACTION_WIDTH = 1280;
	static const int REFRACTION_HEIGHT = 720;

};

