#include "Camera.h"



Camera::Camera()
{
}

Camera::Camera(vec4 position, vec4 at, vec4 up, GLfloat camSpeed, GLfloat camSensitivity)
{
	this->position = position;
	this->at = at;
	this->up = up;
	this->sensitivity = camSensitivity;
	this->speed = camSpeed;
}

mat4 Camera::lookAt()
{
	mat4 modelView = LookAt(position, at, up);

	return modelView;
}

vec4 Camera::getPosition()
{
	return position;
}

vec4 Camera::getAt()
{
	return at;
}

vec4 Camera::getUp()
{
	return up;
}
GLfloat Camera::getSensitivity()
{
	return sensitivity;
}
GLfloat Camera::getSpeed()
{
	return speed;
}
void Camera::setPosition(vec4 position)
{
	this->position = position;
}
void Camera::setAt(vec4 at)
{
	this->at = at;
}
void Camera::setUp(vec4 up)
{
	this->up = up;
}
void Camera::setSensitivity(GLfloat sensitivity)
{
	this->sensitivity = sensitivity;
}
void Camera::setSpeed(GLfloat speed)
{
	this->speed = speed;
}
Camera::~Camera()
{
}
