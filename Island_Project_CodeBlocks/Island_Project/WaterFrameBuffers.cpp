#include "WaterFrameBuffers.h"



WaterFrameBuffers::WaterFrameBuffers()
{
	initialiseReflectionFrameBuffer();
	initialiseRefractionFrameBuffer();
}

 void WaterFrameBuffers::cleanUp() 
 {
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteRenderbuffers(1, &reflectionDepthBuffer);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
	glDeleteTextures(1, &refractionDepthTexture);
}

 GLuint WaterFrameBuffers::getReflectionTexture()
 {
	 return reflectionTexture;
 }

 GLuint WaterFrameBuffers::getRefractionTexture()
 {
	 return refractionTexture;
 }

 GLuint WaterFrameBuffers::getRefractionDepthTexture()
 {
	 return refractionDepthTexture;
 }

 GLuint WaterFrameBuffers::createFrameBuffer()
 {
	 GLuint fb;
	 //generate a name for the framebuffer
	 glGenFramebuffers(1, &fb);
	 //create the framebuffer
	 glBindFramebuffer(GL_FRAMEBUFFER, fb);
	//indicate that we will always render to color attachment 0
	 glDrawBuffer(GL_COLOR_ATTACHMENT0);

	 return fb;
 }

 GLuint WaterFrameBuffers::createTextureAttachment(int width, int height)
 {
	 GLuint texture;
	 //generate a name for the texture
	 glGenTextures(1, &texture);
	 //create the texture object using the name
	 glBindTexture(GL_TEXTURE_2D, texture);
	 //allocate memory for the texture
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 
		 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);

	 return texture;
 }

 GLuint WaterFrameBuffers::createDepthTextureAttachment(int width, int height)
 {
	 GLuint texture;
	 //generate a name for the texture
	 glGenTextures(1, &texture);
	 //create the texture object using the name
	 glBindTexture(GL_TEXTURE_2D, texture);
	 //allocate memory for the texture
	 glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height,
		 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);

	 return texture;

 }

 GLuint WaterFrameBuffers::createDepthBufferAttachment(int width, int height)
 {
	 GLuint depthB;
	 glGenRenderbuffers(1, &depthB);
	 glBindRenderbuffer(GL_RENDERBUFFER, depthB);
	 glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	 glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthB);

	 return depthB;
 }

 void WaterFrameBuffers::bindFrameBuffer(GLuint frameBuffer, int width, int height)
 {
	 //Make sure no there is no texture bounded
	 glBindTexture(GL_TEXTURE_2D, 0);
	 glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	 glViewport(0, 0, width, height);
 }

 void WaterFrameBuffers::bindReflectionFrameBuffer()
 {
	 bindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
 }

 void WaterFrameBuffers::bindRefractionFrameBuffer()
 {
	 bindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);

 }

 void WaterFrameBuffers::unbindCurrentFrameBuffer()
 {
	 glBindFramebuffer(GL_FRAMEBUFFER, 0);
	 glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

 }

 void WaterFrameBuffers::initialiseReflectionFrameBuffer()
 {
	 reflectionFrameBuffer = createFrameBuffer();
	 reflectionTexture = createTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
	 reflectionDepthBuffer = createDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);

	 //Check if the frame buffer is complete
	 if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		 cout << "ERROR::FRAMEBUFFER:: Framebuffer not complete!!" << endl;

	 unbindCurrentFrameBuffer();
 }

 void WaterFrameBuffers::initialiseRefractionFrameBuffer()
 {
	 refractionFrameBuffer = createFrameBuffer();
	 refractionTexture = createTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
	 refractionDepthTexture = createDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);

	 //Check if the frame buffer is complete
	 if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		 cout << "ERROR::FRAMEBUFFER:: Framebuffer not complete!!" << endl;

	 unbindCurrentFrameBuffer();
 }
 WaterFrameBuffers::~WaterFrameBuffers()
{
}
