#pragma once
#include "Angel.h"

class Camera
{
public:
	Camera();
	
	Camera(vec4 position, vec4 at, vec4 up, GLfloat camSpeed, GLfloat camSensitivity);
	//Returns a model view matrix that converts coordinates to eye coordinates
	mat4 lookAt();

	//Returns the camera position
	vec4 getPosition();

	//Returns the camera at
	vec4 getAt();

	//Returns the camera up
	vec4 getUp();

	//Returns the camera sensitivity
	GLfloat getSensitivity();

	//Returns the camera speed
	GLfloat getSpeed();

	//Set the camera position
	void setPosition(vec4 position);

	//Set the camera at
	void setAt(vec4 at);

	//Set the camera up
	void setUp(vec4 up);

	//Set the camera sensitivity
	void setSensitivity(GLfloat sensitivity);

	//Set the camera speed
	void setSpeed(GLfloat speed);

	~Camera();

private:
	vec4 position;
	vec4 at;
	vec4 up;
	GLfloat sensitivity;
	GLfloat speed;
};

