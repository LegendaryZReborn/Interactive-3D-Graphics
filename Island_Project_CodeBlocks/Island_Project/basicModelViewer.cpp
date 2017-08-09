//Name: Cavaughn Browne
// Display a colored model from an obj file
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  (with Perspective Projection)

#include "Angel.h"
#include "Object.h"
#include "MultiMeshObj.h"
#include <fstream>
#define SPACEBAR 32
//#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS_GLOBALS



// Array of rotation angles (in degrees) for each coordinate axis
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;

GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat eyeZ = 4, fov = 45.0, w, h;
const int numModels = 1;
//Object objs[numModels] = {"CavaughnT.obj", "cube.obj", "HelmetHead.obj", "ONE.obj", "Mario.obj" };
MultiMeshObj objs[numModels];
objs[0] = new MultiMeshObj("cube3.obj");
int modelC = 0;

GLuint program;
GLuint  theta;  // The location of the "theta" shader uniform variable
mat4 model_view, rot;
vec4 eye, at;
GLint ModelViewLoc, ProjectionLoc;
//GLuint vao[numModels];
bool rotateT = false, ortho = false;
GLfloat old_x, old_y;

//Lighting
vec4 lightPosition = vec4(5.0, 5.0, 5.0, 0.0); //infinite light source ie vector
vec3 lightAmbient = vec3(1.0, 1.0, 1.0);
vec3 lightDiffuse = vec3(1.5, 1.5, 1.5);
vec3 lightSpecular = vec3(2.0, 2.0, 2.0);

void reshape(int width, int height);



//----------------------------------------------------------------------------
void changeTheta(float oldX, float newX, float oldY, float newY)
{
	float cX, cY;
	cX = (newX - oldX) ;
	cY = (newY - oldY) ;


	//Theta[Axis] += 0.04;
	if(Axis == Xaxis)
		Theta[Axis] += cY;
	else if(Axis == Yaxis)
		Theta[Axis] += cX;
	else if (Axis == Zaxis)
	{
		Theta[Xaxis] += cY;
		Theta[Yaxis] += cX;
	}
	if (Theta[Axis] > 360.0) {
		Theta[Axis] -= 360.0;
	}
	else if (Theta[Axis] < 0)
		Theta[Axis] += 360;


}
//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
	GLuint LDiffuseLoc, LAmbientLoc, LSpecularLoc, LightPositionLoc;



    // Load shaders and use the resulting shader program
    program = InitShader( "vshader_a4.glsl", "fshader_a4.glsl" );
    glUseProgram( program );


	// Create a vertex array object

	//allocates numModel vertex array object names and
	//returns them in array vao
	//glGenVertexArrays(numModels, vao);

	//create a vertex array object for each vertex array
	//name and load an object with each
	for (int m = 0; m < numModels; m++)
	{
	//	glBindVertexArray(vao[m]);
		objs[m].Load(program);
	}

	//activate the vertex array object at index 0
	//glBindVertexArray(objs[modelC].vao);

	//get the location of the "model_view" uniform variable
	ModelViewLoc = glGetUniformLocation(program, "model_view");

	//gets the location of the "proj" uniform variable
	ProjectionLoc = glGetUniformLocation(program, "proj");

	//gets the location of the "theta" uniform variable
   // theta = glGetUniformLocation( program, "theta" );

	LightPositionLoc = glGetUniformLocation(program, "LightPosition");

	LAmbientLoc = glGetUniformLocation(program, "LAmbient");
	LDiffuseLoc = glGetUniformLocation(program, "LDiffuse");
	LSpecularLoc = glGetUniformLocation(program, "LSpecular");


	//send over light settings to the shader
	glUniform3fv(LAmbientLoc, 1, lightAmbient);
	glUniform3fv(LDiffuseLoc, 1, lightDiffuse);
	glUniform3fv(LSpecularLoc, 1, lightSpecular);
	glUniform4fv(LightPositionLoc, 1, lightPosition);



    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	rot = RotateX(Theta[Xaxis]) * RotateY(Theta[Yaxis]) * RotateZ(Theta[Zaxis]);
	eye = vec4(0.0, 0.0, eyeZ, 1.0);
	at = objs[modelC].provideBoxCenter();
	model_view = LookAt(eye, at, vec4(0, 1.0, 0, 1.0));
	model_view *= rot;

	//updates the uniform variable at ModelViewLoc in
	//the shader program with model_view
 	glUniformMatrix4fv(ModelViewLoc, 1, GL_TRUE, model_view);

	//Draws current loaded model
	objs[modelC].Draw(program);

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
	case 033: // Escape Key
	case 'q': case 'Q':
	    exit( EXIT_SUCCESS );
		break;
	case 'o': case 'O':

		if(!ortho)
			eyeZ += 0.1;
		break;
	case 'i': case 'I':
		if (!ortho)
			eyeZ -= 0.1;
		break;
	case 's': case 'S':
		if (!ortho)
			ortho = true;
		else
			ortho = false;

		reshape(w, h);
		break;
	case SPACEBAR:
		eyeZ = 4;
		Theta[Xaxis] = 0.0;
		Theta[Yaxis] = 0.0;
		Theta[Zaxis] = 0.0;


			//switch (modelC)
			//{
			//case 0:
			//	modelC = 1;
			//	//activate vertex array object at index modelC
			//	glBindVertexArray(vao[modelC]);


			//	break;
			//case 1:
			//	modelC = 2;
			//	//activate vertex array object at index modelC
			//	glBindVertexArray(vao[modelC]);


			//	break;
			//case 2:
			//	modelC = 3;
			//	//activate vertex array object at index modelC
			//	glBindVertexArray(vao[modelC]);

			//	break;

			//case 3:
			//	modelC = 4;
			//	//activate vertex array object at index modelC
			//	glBindVertexArray(vao[modelC]);
			//	break;
			//case 4:
			//	modelC = 0;

			//	//activate vertex array object at index modelC
			//	glBindVertexArray(vao[modelC]);
			//	break;

				if (modelC != numModels - 1)
					modelC = (modelC  % numModels) + 1;
				else
					modelC = 0;

				//glBindVertexArray(vao[modelC]);
			//}
		}
}

//----------------------------------------------------------------------------
void reshape(int width, int height)
{
	mat4 proj;

	if (!ortho)
	{
		GLfloat aspect;
		GLfloat zNear, zFar;
		glViewport(0, 0, width, height);
		aspect = GLfloat(width) / GLfloat(height);
		zNear = objs[modelC].provideBoxMax()/3;
		zFar = objs[modelC].provideBoxMax() * 20;

		mat4 proj = Perspective(fov, aspect, zNear, zFar);
		//updates the uniform variable at ProjectionLoc in
		//the shader program with proj
		glUniformMatrix4fv(ProjectionLoc, 1, GL_TRUE, proj);

	}
	else
	{
		GLfloat left, right, top, bottom, zNear, zFar;

		right = objs[modelC].provideBoxMax();
		left = -1 * right;
		top = objs[modelC].provideBoxMax();
		bottom = -1 * top;
		zNear = objs[modelC].provideBoxMax() * 0.5;
		zFar = zNear * 20;

		mat4 proj = Ortho(left, right, bottom, top, zNear, zFar);

		//updates the uniform variable at ProjectionLoc in
		//the shader program with proj
		glUniformMatrix4fv(ProjectionLoc, 1, GL_TRUE, proj);

	}

	w = width;
	h = height;
}
//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
		switch( button ) {
			case GLUT_LEFT_BUTTON:    Axis = Xaxis;
				//changeTheta();
				break;
			case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;
				//changeTheta();
				break;
			case GLUT_RIGHT_BUTTON:   Axis = Zaxis;
				//changeTheta();
				break;
		}
		rotateT = true;
		old_x = x;
		old_y = y;
    }
	else
	{
		rotateT = false;
	}
}

//----------------------------------------------------------------------------

void mouseMove(int x, int y)
{
	if (rotateT)
	{
		changeTheta(old_x, x, old_y, y);
		old_x = x;
		old_y = y;
	}
}
//----------------------------------------------------------------------------

void mouseWheelScroll(int button, int dir, int x, int y)
{
	if (!ortho)
	{
		if (dir > 0)
		{
			//zoom in
			eyeZ -= 0.1;
		}
		else
		{
			//zoom out
			eyeZ += 0.1;
		}
	}
}

//----------------------------------------------------------------------------
void
idle( void )
{
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
	w = 512;
	h = 512;
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Model Viewer" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
	glutReshapeFunc(reshape);
    glutMouseFunc( mouse );
	glutMotionFunc(mouseMove);
	glutMouseWheelFunc(mouseWheelScroll);
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
