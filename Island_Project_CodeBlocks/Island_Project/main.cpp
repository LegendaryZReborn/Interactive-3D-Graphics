//Name: Cavaughn Browne
//Island Temple Project - Water, Frustrum Culling (Only on temple's objects for now),
//detail texture on terrain

//Press I/O to increase/ decrease frequency of water.
//Mouse scroll to increase and decrease speed.
#define __USE_MINGW_ANSI_STDIO 0

#pragma once

#include "Object.h"
#include "MultiMeshObj.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Camera.h"
#include "DirLight.h"
#include "PointLight.h"
#include <fstream>
#include <ctime>
#include "float.h"
#include <cmath>
#include<vector>
#include <random>
#include "Water.h"
#include "Temple.h"
#include <algorithm>



#define SPACEBAR 32
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS_GLOBALS

enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;

GLfloat fov = 45.0, aspect = 1.0;
GLfloat deltaTime = 0.0;
GLfloat lastframe = 0.0;
GLfloat frame = 0.0, timeC, timebase = 0.0, fps;
mat4 model_view, proj;

//Camera
Camera camera(vec4(0.0, 0.0, 10.0, 1.0), vec4(0.0, 0.0, -1.0, 0.0),
	vec4(0.0, 1.0, 0.0, 1.0), 0.0, 0.5);
vec4 eye, at, up;
GLfloat currentCamSpeed = 0.1;

//Other
bool rotateT = false, firstMouse = true;
GLfloat old_x, old_y, yaw = 0, pitch = 0, waveL = 0.0;

//Global Light sources for everything in the scene
vector<DirLight> Lights;
vector<PointLight> ptempleLights;

//programs
GLuint program_object, program_water, program_terrain;

//Scene objects
Temple temple;
Terrain t("firstTerrain2.jpg", true);
Skybox sky;
Water waterQuad(1024, false, "");
MultiMeshObj obj("ONE.obj");

// OpenGL initialization
void init();
void display(void);
void reshape(int width, int height);
void idle(void);
void drawScene();
void calculateDeltaTime();
void displayFrameRate();
bool checkFrustrum(vector<vec4> bBPoints);
void transferLightsToShader();
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void calcYawPitch(float oldX, float newX, float oldY, float newY);
void mouseMove(int x, int y);
void mouseWheelScroll(int button, int dir, int x, int y);


//----------------------------------------------------------------------------

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Cavaughn Browne - Island Temple");

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
    //Initialize the shaders to be used
	program_object = InitShader("vshader2.glsl", "fshader_a4.glsl");
    program_water = InitShader("watervShader.glsl", "waterfShader.glsl");
    program_terrain =InitShader("vshader_terrain.glsl", "fshader_terrain.glsl");

	vec3 lightAmbient = vec3(1.0, 1.0, 1.0);
	vec3 lightDiffuse = vec3(1.0, 1.0, 1.0);
	vec3 lightSpecular = vec3(1.0, 1.0, 1.0);
	vec4 position = vec4(-10.0, 100.0, 0.0, 0.0);
    DirLight light = DirLight(position, lightAmbient, lightDiffuse, lightSpecular);
	Lights.push_back(light);

	sky.Load();	                     //Loads the skybox
	waterQuad.load(program_water);   //Load the water quad
	waterQuad.setReflectionTex(sky.sendSkyTex());
	t.Load(program_terrain); 	    //Loads the terrain
    temple.load(program_object);    //Loads the temple
    obj.Load(program_object);
	 transferLightsToShader();


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_SCISSOR_TEST);
	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	calculateDeltaTime();
	drawScene();
	displayFrameRate();
	glutSwapBuffers();
}

//----------------------------------------------------------------------------
void reshape(int width, int height)
{
	GLfloat zNear, zFar;
	glViewport(0, 0, width, height);
	aspect = GLfloat(width) / GLfloat(height);
	zNear = 0.01;
	zFar = 1000;
	fov = 45.0;

	proj = Perspective(fov, aspect, zNear, zFar);


}
//----------------------------------------------------------------------------

void
idle(void)
{
	glutPostRedisplay();
}



//----------------------------------------------------------------------------
void drawScene()
{

	mat4 scale = Scale(vec3(1.0, 1.0, 1.0));
	eye = camera.getPosition();
	at = eye + camera.getAt();
	up = camera.getUp();

	model_view = LookAt(eye, at, up);
    transferLightsToShader();

    GLuint NormalViewLoc = glGetUniformLocation(program_object, "normalMatrix");

    mat3 normalMatrix = Normal(model_view);
    glUniformMatrix3fv(NormalViewLoc, 1, GL_TRUE, normalMatrix);

    sky.Draw();
    t.Draw();
    temple.translateTemple(vec3(-7.0, 61.5, 20.0));
    temple.draw();
    waterQuad.translateWater(vec3(0.0, 10.0, 0.0));
    waterQuad.draw();
    obj.translateObj(vec3(-7.0, 66.5, 20.0));
    obj.Draw();
}

//----------------------------------------q------------------------------------

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

//Checks if a bounding box intersects with the frustrum
bool checkFrustrum(vector<vec4> bBPoints)
{
	//Define frustrum box
	vec4 frustrumMinPoint = vec4(-1, -1, -1, 1);
	vec4 frustrumMaxPoint = vec4(1, 1, 1, 1);

	//Define objects bounding box in screen coordinates
	for (auto &x : bBPoints)
	{
		x = proj * model_view * x;
		x /= abs(x.w);
	}

	GLfloat min_x = FLT_MAX, min_y = FLT_MAX, min_z = FLT_MAX, max_x = -FLT_MAX,
		max_y = -FLT_MAX, max_z = -FLT_MAX;


	//Find the AABB of new model_view by finding all minimums and maximums first
	for (auto &point : bBPoints)
	{
		min_x = min(point.x, min_x);
		min_y = min(point.y, min_y);
		min_z = min(point.z, min_z);
		max_x = max(point.x, max_x);
		max_y = max(point.y, max_y);
		max_z = max(point.z, max_z);
	}

	//Check if the two boxes intersect
	//Assuming object is on the left
	if (min_x > frustrumMaxPoint.x || frustrumMinPoint.x > max_x)
		return false;

	//Assuming object is above
	if (min_y > frustrumMaxPoint.y || frustrumMinPoint.y > max_y)
		return false;

	//Assuming object is in front
	if (min_z > frustrumMaxPoint.z || frustrumMinPoint.z > max_z)
		return false;

	return true;

}

//----------------------------------------------------------------------------

void transferLightsToShader()
{

    //Transfer the directional and point lights to program_object
    string l;
	l = "lights[0].";

	for (int i = 0; i < Lights.size(); i++)
	{
	    //modify the index number in l to character version of i
		l[7] = i + '0';

        Lights[i].init(program_object, l + "LightPosition", l + "LAmbient",
                    l + "LDiffuse", l + "LSpecular");

		//send over light settings to the shaders of the specified program
		Lights[i].transferSettings(program_object);
	}

	l = "pLight[0].";

	for (int i = 0; i < ptempleLights.size(); i++)
	{
        //modify the index number in l to character version of i
		l[7] = i + '0';

        ptempleLights[i].init(program_object, l + "LightPosition", l + "LAmbient",
			l + "LDiffuse", l + "LSpecular", l + "constant", l + "linear", l + "quadratic");
		//send over light settings to the shaderS
		ptempleLights[i].transferSettings(program_object);
	}


    glUniform1i(glGetUniformLocation(program_object, "NUM_LIGHTS"), Lights.size());
	glUniform1i(glGetUniformLocation(program_object, "NUM_PLIGHTS"), ptempleLights.size());
}

//----------------------------------------------------------------------------

void
keyboard(unsigned char key, int x, int y)
{
	GLfloat cameraSpeed;
	vec4 cameraPos, cameraFront, cameraUp;
	vec4 lightP;


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
    case 'u':

        for (int pL = 0; pL < ptempleLights.size(); pL++)
        {
            lightP =  ptempleLights[pL].getPosition() + vec4(0.0, 1.0, 0.0, 0.0);
            ptempleLights[pL].setPosition(lightP);
        }

        break;
    case 'j':

        for (int pL = 0; pL < ptempleLights.size(); pL++)
        {
            lightP =  ptempleLights[pL].getPosition() + vec4(0.0, -1.0, 0.0, 0.0);

            cout << ptempleLights[pL].getPosition().y << endl;
            ptempleLights[pL].setPosition(lightP);
            cout << ptempleLights[pL].getPosition().y << endl;
        }
        break;
    case 'h':

        for (int pL = 0; pL < ptempleLights.size(); pL++)
        {
            lightP =  ptempleLights[pL].getPosition() + vec4(0.0, 0.0, 1.0, 0.0);
            ptempleLights[pL].setPosition(lightP);
        }
        break;
    case 'k':

        for (int pL = 0; pL < ptempleLights.size(); pL++)
        {
            lightP =  ptempleLights[pL].getPosition() + vec4(0.0, 0.0, -1.0, 0.0);
            ptempleLights[pL].setPosition(lightP);
        }
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
