#include "Skybox.h"
#include "Angel.h"
extern vec4 eye, at, up;
extern GLfloat aspect;
extern GLfloat fov;
int skyprojection_loc;
mat4 skyprojection;
GLfloat bsize = 200.0;//Size of Cube
GLfloat points[] = {
	-bsize,bsize,-bsize,
	-bsize,-bsize,-bsize,
	bsize,-bsize,-bsize,
	bsize,-bsize,-bsize,
	bsize,bsize,-bsize,
	-bsize,bsize,-bsize,
	-bsize, -bsize,  bsize,
	-bsize, -bsize, -bsize,
	-bsize,  bsize, -bsize,
	-bsize,  bsize, -bsize,
	-bsize,  bsize,  bsize,
	-bsize, -bsize,  bsize,
	bsize, -bsize, -bsize,
	bsize, -bsize,  bsize,
	bsize,  bsize,  bsize,
	bsize,  bsize,  bsize,
	bsize,  bsize, -bsize,
	bsize, -bsize, -bsize,
	-bsize, -bsize,  bsize,
	-bsize,  bsize,  bsize,
	bsize,  bsize,  bsize,
	bsize,  bsize,  bsize,
	bsize, -bsize,  bsize,
	-bsize, -bsize,  bsize,
	-bsize,  bsize, -bsize,
	bsize,  bsize, -bsize,
	bsize,  bsize,  bsize,
	bsize,  bsize,  bsize,
	-bsize,  bsize,  bsize,
	-bsize,  bsize, -bsize,
	-bsize, -bsize, -bsize,
	-bsize, -bsize,  bsize,
	bsize, -bsize, -bsize,
	bsize, -bsize, -bsize,
	-bsize, -bsize,  bsize,
	bsize, -bsize,  bsize
};


Skybox::Skybox(void)
{
}


Skybox::~Skybox(void)
{
}


void Skybox::Load(void)
{
	std::cout << "Initializing skybox" << std::endl;
	//Compiler the shaders for this skybox and build associated program
	program_sky = InitShader("vshader_sky.glsl", "fshader_sky.glsl");
	glUseProgram(program_sky);

	// Define the cube map vao
	glGenVertexArrays(1, &vao_cube);
	glBindVertexArray(vao_cube);
	glGenBuffers(1, &cubebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	skyprojection_loc = glGetUniformLocation(program_sky, "P");
	skyprojection = Perspective(fov, aspect, 1, 500.0);
	//Send over the projection matrix
	glUniformMatrix4fv(skyprojection_loc, 1, GL_FALSE, skyprojection);
	// Cube Map Texture Buffers and loading
	GLuint skyPosition = glGetAttribLocation(program_sky, "skyPosition");
	glVertexAttribPointer(skyPosition, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glEnableVertexAttribArray(skyPosition);

	sky_mv_loc = glGetUniformLocation(program_sky, "MV");
	TextureLoc = glGetUniformLocation(program_sky, "cube_tex");//tex map in frag shader
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &tex);

	//char * cubefaces[6] = {"Skybox\\right1.png", "Skybox\\left1.png", "Skybox\\top1.png","Skybox\\bottom1.png" ,
	//	"Skybox\\back1.png", "Skybox\\front1.png" };
	char * cubefaces[6] = { "Skybox\\sea_rt.jpg", "Skybox\\sea_lf.jpg", "Skybox\\sea_up.jpg","Skybox\\sea_dn.jpg" ,
		"Skybox\\sea_bk.jpg", "Skybox\\sea_ft.jpg" };
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	unsigned char* image;
	//read in the textures and load them on the GPU
	for (int i = 0; i<6; i++) {
		image = SOIL_load_image(cubefaces[i], &nWidth, &nHeight, 0, SOIL_LOAD_RGB);
		if (!image) {
			std::cout << "ERROR: could not load " << cubefaces[i] << std::endl;
		}
		if ((nWidth & (nWidth - 1)) != 0 || (nHeight & (nHeight - 1)) != 0) {
			std::cout << "WARNING: image " << cubefaces[i] << " is not power-of-2 dimensions" << std::endl;
		}
		GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		glTexImage2D(target, 0, GL_RGB, nWidth, nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
}


void Skybox::Draw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);

	glUseProgram(program_sky);

	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	mat4 MV = LookAt(eye, at, up);

	for (int i = 0; i < 4; i++)
		MV[i].w = 0;

	MV[3].x = 0;
	MV[3].y = 0;
	MV[3].z = 0;
	MV[3].w = 1;

	
	glUniformMatrix4fv(sky_mv_loc, 1, GL_TRUE, MV);
	glBindVertexArray(vao_cube);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthMask(GL_TRUE);
}

void Skybox::sendSkyTex(GLuint program)
{
	GLuint skyBoxTexLoc;

	glUseProgram(program);
	skyBoxTexLoc = glGetUniformLocation(program, "skyboxTex");

	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	glUniform1ui(skyBoxTexLoc, tex);



}