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

// Array of rotation angles (in degrees) for each coordinate axis
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };



GLfloat aspect = 16.0 /9.0, fov = 45.0, w, h;
const int numModels = 4;
//MultiMeshObj objs[numModels] = { "GreekTempleRoof.obj", "GreekTemplePillar2.obj", 
//"GreekTempleStairs.obj" , "GreekTempleFloor.obj" };
Object quad("quad.obj");
//Material mat(96.0, 1, 1, vec3(0.5, 0.5, 0.5), vec3(0.0, 1.0, 0.0), vec3(0.500000, 0.500000, 0.500000), 2, "");
//Terrain water("waterQuad.jpg", false, mat, 1024);
Terrain t("firstTerrain2.jpg", true);
Skybox sky;
//MultiMeshObj cube("cube4tex.obj");
MultiMeshObj cube2("cube3.obj");
//MultiMeshObj cube3("cube6tex.obj");



int modelC = 0;

GLfloat deltaTime = 0.0;
GLfloat lastframe = 0.0;
GLfloat frame = 0.0, timeC, timebase = 0.0, fps;

GLuint program, waterProgram;
GLuint  theta;  // The location of the "theta" shader uniform variable
mat4 model_view, proj;


//Camera
Camera camera(vec4(0.0, 10.0, 35.0, 1.0), vec4(0.0, 0.0, -1.0, 0.0),
	vec4(0.0, 1.0, 0.0, 1.0), 0.0, 0.5);

vec4 eye = camera.getPosition();
vec4 at = camera.getAt();
vec4 up = camera.getUp();
GLfloat zz = -2;
GLint ModelViewLoc, ProjectionLoc, translateLoc, ScaleLoc, deltaTimeLocV;
GLint ModelViewLoc2, ProjectionLoc2, translateLoc2, ScaleLoc2, deltaTimeLoc;

bool rotateT = false, firstMouse = true;
GLfloat old_x, old_y;
GLfloat yaw  = 0 , pitch = 0;

//A Light structure. Each light has a position, ambient, diffuse and specular values.
struct Light
{
	Light(vec4 pos, vec3 a, vec3 d, vec3 s)
	{
		lightPosition = pos;
		lightAmbient = a;
		lightDiffuse = d;
		lightSpecular = s;
	}

	vec4 lightPosition;

	vec3 lightAmbient; 
	vec3 lightDiffuse; 
	vec3 lightSpecular;
};

GLfloat waveL = 0;
GLuint waveLLoc;
//for pillar data
map<string, vector<vec4>> locations;
map<string, vector<vec3>> rotations;
map<string, vector<vec3>> scales;
string datPath = "Files\\placement.dat";




void readObjectDataFile(string name);

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

void readObjectDataFile(string name)
{
	ifstream infile;
	vec4 temp;
	vec3 temp2, temp3;
	string Oname = "", nOname = "";
	char c;

	vector<vec4> l;
	vector<vec3> r;
	vector<vec3> s;


	infile.open(name);
	infile >> Oname;
	nOname = Oname;
	while (infile)
	{
		while (infile && (nOname == Oname))
		{
			infile >> temp.x >> temp.y >> temp.z;
			temp.w = 1;

			l.push_back(temp);

			infile >> temp2.x >> temp2.y >> temp2.z;
			r.push_back(temp2);


			infile >> temp3.x >> temp3.y >> temp3.z;
			s.push_back(temp3);

			infile >> nOname;
		}

		locations.insert(pair<string, vector<vec4>>(Oname, l));
		rotations.insert(pair<string, vector<vec3>>(Oname, r));
		scales.insert(pair<string, vector<vec3>>(Oname, s));
		l.clear();
		r.clear();
		s.clear();

		Oname = nOname;
	}
	infile.close();

}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{

	// Load shaders and use the resulting shader program
	program = InitShader("vshader2.glsl", "fshader_a4.glsl");
	waterProgram = InitShader("watervShader.glsl", "waterfShader.glsl");

	//Read location data for objects in scene from a file
	readObjectDataFile(datPath);
	
	//Inializes/Loads the skybox
	sky.Load();
	sky.sendSkyTex(waterProgram);

	
	//load each object
	//cube.Load(program);
	cube2.Load(program);
	//cube3.Load(program);

	vec3 lightAmbient = vec3(0.7, 0.5, 0.7);
	vec3 lightDiffuse = vec3(0.3, 0.3, 0.7);
	vec3 lightSpecular = vec3(1.0, 1.0, 1.0);

	vector<DirLight> Lights;

	//Lights.push_back(vec4(0.0, 0.0, 0.0, 0.0));
	Lights.push_back(DirLight(vec4(15.0, 15.0, 15.0, 1.0), lightAmbient, lightDiffuse, lightSpecular));
	//Lights.push_back(Light(vec4(10.0, -50.0, -1024.0, 1.0), lightAmbient, lightDiffuse, lightSpecular));
	//Lights.push_back(Light(vec4(0.0, 500.0, 0.0, 1.0), vec3(1.0, 1.0, 1.0), vec3(0.0, 1.0, 0.0), lightSpecular));
	//Lights.push_back(vec4(1000.0, 1.0, 0.0, 0.0));

	//Loads the terrain
	t.Load(program);

	//get the location of the "model_view" uniform variable
	ModelViewLoc = glGetUniformLocation(program, "model_view");
	//get the location of the "translate" uniform variable
	translateLoc = glGetUniformLocation(program, "translate");
	//gets the location of the "proj" uniform variable
	ProjectionLoc = glGetUniformLocation(program, "proj");
	//gets the location of the "scale" uniform variable
	ScaleLoc = glGetUniformLocation(program, "scale");

	deltaTimeLocV = glGetUniformLocation(program, "t");


	//send each member of each light to its appropriate place in the shader
	string l = "lights[0].";
	string p;
	for (int i = 0; i < Lights.size(); i++)
	{
		l[7] = i + '0';

		Lights[i].init(program, l + "LightPosition", l + "LAmbient",
			l + "LDiffuse", l + "LSpecular");
	
		//send over light settings to the shader
		Lights[i].transferSettings(program);
	}

	//Send Point Light
	PointLight pLight(vec4(0.0, 15.0, -25.0, 1.0), vec3(1.0, 0.5, 0.5), 
		vec3(1.0, 0.0, 0.0), vec3(1.0, 0.4, 0.4), 0.02, 0.02, 0.008);

	l = "pLight.";
	pLight.init(program, l + "LightPosition", l + "LAmbient",
		l + "LDiffuse", l + "LSpecular", l + "constant", l + "linear", l + "quadratic");


	//send over light settings to the shader
	pLight.transferSettings(program);

	quad.Load(waterProgram);
//	water.Load(waterProgram);
	Lights.push_back(DirLight(vec4(-15.0, 5.0, -15.0, 1.0), lightAmbient, lightDiffuse, vec3(0.1, 0.1, 0.8)));

	GLuint amplitudeLoc, speedLoc, directionLoc;

	//get the location of the "model_view" uniform variable
	ModelViewLoc2 = glGetUniformLocation(waterProgram, "model_view");
	//get the location of the "translate" uniform variable
	translateLoc2 = glGetUniformLocation(waterProgram, "translate");
	//gets the location of the "proj" uniform variable
	ProjectionLoc2 = glGetUniformLocation(waterProgram, "proj");
	//gets the location of the "scale" uniform variable
	ScaleLoc2 = glGetUniformLocation(waterProgram, "scale");
	deltaTimeLoc = glGetUniformLocation(waterProgram, "t");
	amplitudeLoc = glGetUniformLocation(waterProgram, "amplitude");
	speedLoc = glGetUniformLocation(waterProgram, "speed");
	directionLoc = glGetUniformLocation(waterProgram, "direction");
	waveLLoc = glGetUniformLocation(waterProgram, "f");
	 l = "lights[0].";
	//send each member of each light to its appropriate place in the shader
	 for (int i = 0; i < Lights.size(); i++)
	 {
		 l[7] = i + '0';

		 Lights[i].init(waterProgram, l + "LightPosition", l + "LAmbient",
			 l + "LDiffuse", l + "LSpecular");

		 //send over light settings to the shader
		 Lights[i].transferSettings(waterProgram);
	 }

	GLfloat amplitude[8];
	GLfloat speed[8];
	vec2 direction[8];
	srand(time(NULL));
	for (int i = 0; i < 8; ++i)
	{
		amplitude[i] = 1;
		//waveL = 0;
		//speed[i] = (rand() % 5 + 1) /4.5;
		speed[i] = 5;
		direction[i] = vec2(rand() % 3 - 1, rand() % 3 - 1);

	}
	glUniform1fv(amplitudeLoc, 8, amplitude);
	glUniform1f(waveLLoc, waveL);
	glUniform1fv(speedLoc, 8, speed);
	glUniform2fv(directionLoc, 8, direction[0]);

	//vector<vec4> vertices = quad.getVertices();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_SCISSOR_TEST);
	glEnable(GL_BLEND);

	glClearColor(1.0, 1.0, 1.0, 1.0);
}

//----------------------------------------------------------------------------
void
display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	calculateDeltaTime();


	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
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
	//updates the uniform variable at ProjectionLoc in
	//the shader program with proj
	glUniformMatrix4fv(ProjectionLoc, 1, GL_TRUE, proj);
	glUniformMatrix4fv(ProjectionLoc2, 1, GL_TRUE, proj);


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

	sky.Draw();

	model_view = LookAt(eye, at, up);


	glUseProgram(program);

	glUniform1f(deltaTimeLocV, glutGet(GLUT_ELAPSED_TIME) / 200);

	//Draw terrain
	t.Draw(program);

	cube2.Draw(program);

	glUseProgram(waterProgram);

//	glUniformMatrix4fv(ModelViewLoc2, 1, GL_TRUE, model_view);
	//glUniformMatrix4fv(ScaleLoc2, 1, GL_TRUE, scale);
	//translationM = Translate(vec3(0.0, 5.0, 0.0));
	//glUniformMatrix4fv(translateLoc2, 1, GL_TRUE, translationM);
	glUniform1f(deltaTimeLoc, glutGet(GLUT_ELAPSED_TIME)/100);
	glUniform1f(waveLLoc, waveL);
	
	quad.translateObj(vec3(0.0, 5.0, 0.0));
	quad.Draw(waterProgram);

	glUseProgram(program);


	


/*	//Draws models
	for (auto x : objs)
	{

		if (x.getFileName() == "GreekTemplePillar2.obj")
		{
			vector<vec4> l = locations["Pillars"];

			for (int i = 0; i < l.size(); i++)
			{
				translationM = Translate(l[i].x, l[i].y , l[i].z) ;
				glUniformMatrix4fv(translateLoc, 1, GL_TRUE, translationM);


				x.Draw(program);

			}
		}
		else if (x.getFileName() == "GreekTempleFloor.obj")
		{
			vector<vec4> l = locations["Floors"];

			for (int i = 0; i < l.size(); i++)
			{
				translationM = Translate(l[i].x, l[i].y, l[i].z);
				glUniformMatrix4fv(translateLoc, 1, GL_TRUE, translationM);

				x.Draw(program);
			}
		}
		else if (x.getFileName() == "GreekTempleRoof.obj")
		{
			vector<vec4> l = locations["Roof"];

			for (int i = 0; i < l.size(); i++)
			{
				translationM = Translate(l[i].x, l[i].y , l[i].z);
				glUniformMatrix4fv(translateLoc, 1, GL_TRUE, translationM);

				x.Draw(program);
			}
		}
		else if (x.getFileName() == "GreekTempleStairs.obj")
		{
			vector<vec4> l = locations["Stairs"];

			for (int i = 0; i < l.size(); i++)
			{
				translationM = Translate(l[i].x, l[i].y, l[i].z);
				glUniformMatrix4fv(translateLoc, 1, GL_TRUE, translationM);


				x.Draw(program);
			}
		}


	}
	*/
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

	camera.setSpeed(0.02 * deltaTime);
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
		waveL += 0.01;
		break;

	case 'o': case 'O':
		if((waveL - 0.01) > 0)
			waveL -= 0.01;
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
			//changeTheta();
			rotateT = true;
			break;
		case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;
			//changeTheta();
			break;
		case GLUT_RIGHT_BUTTON:   Axis = Zaxis;
			//changeTheta();

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
		//zoom in

		if (fov >= 1.0f)
			fov -= 1;

		if (fov <= 1.0f)
			fov = 1.0f;
		reshape(w, h);

	}
	else
	{
		//zoom out 
		if (fov <= 45.0f)
			fov += 1;
		if (fov >= 45.0f)
			fov = 45.0f;

		reshape(w, h);

	}
}

//----------------------------------------------------------------------------
