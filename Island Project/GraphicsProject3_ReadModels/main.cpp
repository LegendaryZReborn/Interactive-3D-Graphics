//Name: Cavaughn Browne
// Display a colored model from an obj file
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  (with Perspective Projection)

#include "Angel.h"
#include "Object.h"
#include "MultiMeshObj.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Camera.h"
#include "WaterFrameBuffers.h"
#include "DirLight.h"
#include "PointLight.h"
#include <fstream>
#include <ctime>
#include <cmath>
#include<vector>
#include <random>
#include "Water.h"
#include "Temple.h"

// following commands are useable in VS ONLY!!
// don't forget to add Include Directories and Library Directories in project Property Pages
// -> Configuration Properties -> VC++ Directories
#pragma comment(lib, "freeglut")	// <- tells compiler to look for freeglut.lib
#pragma comment(lib, "glew32")		// <- tells compiler to look for glew32.lib
#define SPACEBAR 32
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS_GLOBALS

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;

GLfloat aspect = 16.0 /9.0, fov = 45.0, w, h;
//Terrain t("firstTerrain2.jpg", true);
Skybox sky;
//MultiMeshObj cube2("cube6tex.obj");
Water waterQuad(1024, false, "");


GLfloat deltaTime = 0.0;
GLfloat lastframe = 0.0;
GLfloat frame = 0.0, timeC, timebase = 0.0, fps;

mat4 model_view, proj;


//Camera
Camera camera(vec4(0.0, 10.0, 35.0, 1.0), vec4(0.0, 0.0, -1.0, 0.0),
	vec4(0.0, 1.0, 0.0, 1.0), 0.0, 0.5);

vec4 eye = camera.getPosition();
vec4 at = camera.getAt();
vec4 up = camera.getUp();
GLfloat currentCamSpeed = 0.001;
GLfloat zz = -2;
GLfloat waveL = 0.0;

bool rotateT = false, firstMouse = true;
GLfloat old_x, old_y;
GLfloat yaw  = 0 , pitch = 0;

//Global Light sources for everything in the scene
vector<DirLight> Lights;


Temple temple;
// OpenGL initialization
void init();

void display(void);

void reshape(int width, int height);

void idle(void);

void renderScene();

void calculateDeltaTime();

void displayFrameRate();

void keyboard(unsigned char key, int x, int y);

void mouse(int button, int state, int x, int y);

void calcYawPitch(float oldX, float newX, float oldY, float newY);

void mouseMove(int x, int y);

void mouseWheelScroll(int button, int dir, int x, int y);


//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	w = 512;
	h = 512;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Model Viewer");

	glewExperimental = GL_TRUE;
	glewInit();
	

	
	init();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	glutMouseWheelFunc(mouseWheelScroll);
	glutIdleFunc(idle);

	glutMainLoop();
	return 0;
}



//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{

	// Load shaders and use the resulting shader program
	vec3 lightAmbient = vec3(0.3, 0.3, 0.7);
	vec3 lightDiffuse = vec3(0.3, 0.3, 0.7);
	vec3 lightSpecular = vec3(1.0, 1.0, 1.0);

	Lights.push_back(DirLight(vec4(15.0, 15.0, 15.0, 1.0), lightAmbient, lightDiffuse, lightSpecular));

	lightAmbient = vec3(0.8, 0.8, 1.0);
	lightDiffuse = vec3(2.6, 1.6, 1.6);
	Lights.push_back(DirLight(vec4(15.0, 15.0, 15.0, 1.0), lightAmbient, lightDiffuse, lightSpecular));
	
	//Inializes/Loads the skybox
	sky.Load();
	
	//Load the water quad
	waterQuad.load();
	waterQuad.setReflectionTex(sky.sendSkyTex());

	//cube2.Load();
	//Loads the terrain
	//t.Load();
	temple.load();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	//glEnable(GL_SCISSOR_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
void
display(void)
{
	//glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	calculateDeltaTime();


	//reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	//glScissor(0, 0,glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	renderScene();
	
	//reshape(200, 200);
	//glScissor(0, 0, 200, 200);
	//renderScene();

	displayFrameRate();
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
void reshape(int width, int height)
{
	GLfloat aspect;
	GLfloat zNear, zFar;
	glViewport(0, 0, width, height);
	aspect = GLfloat(width) / GLfloat(height);
	zNear = 1.0;
	zFar = 1000000;

	proj = Perspective(fov, aspect, zNear, zFar);

	w = width;
	h = height;
}
//----------------------------------------------------------------------------

void
idle(void)
{
	glutPostRedisplay();
}



//----------------------------------------------------------------------------
void renderScene()
{

	mat4 translationM;
	mat4 scale = Scale(vec3(1.0, 1.0, 1.0));


	eye = camera.getPosition();
	at = eye + camera.getAt();
	up = camera.getUp();
	model_view = LookAt(eye, at, up);

	sky.Draw();

	//Draw terrain
	//t.Draw();
	temple.draw();

	//waterQuad.translateWater(vec3(0.0, 10.0, 0.0));
	//waterQuad.draw();


	
}

//----------------------------------------------------------------------------

//Calculate delta time - time it took for last frame to render
void calculateDeltaTime()
{
	GLfloat curr;
	curr = glutGet(GLUT_ELAPSED_TIME) / 1000;
	deltaTime = curr - lastframe;
	lastframe = deltaTime;
}

//----------------------------------------------------------------------------

//Calculate and display frame rate of application
void displayFrameRate()
{
	frame++;
	timeC = glutGet(GLUT_ELAPSED_TIME);
	string dis;

	if (timeC - timebase > 1000)
	{
		fps = frame * 1000.0 / (timeC - timebase);
		dis = " Cavaughn Browne's Island Temple Project: FPS = " + to_string(fps);
		glutSetWindowTitle(dis.c_str());
		timebase = timeC;
		frame = 0;
	}
}

//----------------------------------------------------------------------------


void
keyboard(unsigned char key, int x, int y)
{
	GLfloat cameraSpeed;
	vec4 cameraPos, cameraFront, cameraUp;

	if (currentCamSpeed < 0)
		currentCamSpeed = 0.001;

	camera.setSpeed(currentCamSpeed * deltaTime);
	cameraSpeed = camera.getSpeed();
	cameraPos = camera.getPosition();
	cameraFront = camera.getAt();
	cameraUp = camera.getUp();

	switch (key) {
	case 033: // Escape Key
	case 'q': case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 's': case 'S':

		cameraPos -= cameraSpeed * cameraFront;
		camera.setPosition(cameraPos);
		break;
	case 'w': case 'W':
		cameraPos += cameraSpeed * cameraFront;
		camera.setPosition(cameraPos);

		break;

	case 'a': case 'A':
		cameraPos -= cross(cameraFront, cameraUp) * cameraSpeed;
		camera.setPosition(cameraPos);

		break;

	case 'd': case 'D':
		cameraPos += cross(cameraFront, cameraUp) * cameraSpeed;
		camera.setPosition(cameraPos);
		break;

	case 'i': case 'I':
		waveL += 0.001;
		waterQuad.setFrequency(waveL);
		break;

	case 'o': case 'O':
		if ((waveL - 0.001) > 0)
		{
			waveL -= 0.001;
			waterQuad.setFrequency(waveL);
		}
		break;

	case 'r':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		break;
	case 'R':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		break;

	case SPACEBAR:

		break;

	}
}


//----------------------------------------------------------------------------

void
mouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN) {
		switch (button) {
		case GLUT_LEFT_BUTTON:    Axis = Xaxis;
			rotateT = true;
			break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;
			break;
		case GLUT_RIGHT_BUTTON:   Axis = Zaxis;
			break;
		}


	}
	else
	{
		rotateT = false;
		firstMouse = true;
	}
}

//----------------------------------------------------------------------------
void calcYawPitch(float oldX, float newX, float oldY, float newY)
{
	float cX, cY;

	cX = (newX - oldX);
	cY = (newY - oldY);

	yaw += (cX * camera.getSensitivity());
	pitch += (-1 * cY * camera.getSensitivity());

	//restrict how much the camera can look up or down
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

}

//----------------------------------------------------------------------------

void mouseMove(int x, int y)
{
	if (rotateT)
	{
		if (firstMouse)
		{
			old_x = x;
			old_y = y;
			firstMouse = false;
		}

		calcYawPitch(old_x, x, old_y, y);
		old_x = x;
		old_y = y;

		vec4 front;
		front.x = sin(DegreesToRadians * yaw) * cos(DegreesToRadians * pitch);
		front.y = sin(DegreesToRadians *pitch);
		front.z = -cos(DegreesToRadians *yaw) * cos(DegreesToRadians *pitch);
		front.w = 0.0;

		camera.setAt(normalize(front));
	}

}
//----------------------------------------------------------------------------

void mouseWheelScroll(int button, int dir, int x, int y)
{
	if (dir > 0)
	{

		currentCamSpeed += 0.001;

	}
	else
	{
		
		if ((currentCamSpeed -= 0.001) > 0)
			currentCamSpeed -= 0.001;

	}
}

//----------------------------------------------------------------------------
