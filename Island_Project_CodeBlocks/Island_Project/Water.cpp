#include "Water.h"
#include "Soil.h"
#include "DirLight.h"
#include <random>
#include <time.h>

extern mat4 model_view;
extern vector<DirLight> Lights;
extern mat4 proj;

Water::Water()
{
	size = 4096;
	frequency = 0;
}

Water::Water(int size, bool hasTexture, string texFile)
{
	this->size = size;

	ctr_box = vec4(size/2.0, 0, size/2.0, 1.0);
	this->hasTexture = hasTexture;

	mat.Shininess = 96.0;
	mat.mAmbient = vec3(0.00000, 0.0000, 0.00000);
	mat.mDiffuse = vec3(0.000000 ,0.000000, 1.00000);
	mat.mSpecular = vec3(0.000000, 0.500000, 0.800000);
	mat.mOpticalDensity = 1.000000;
	mat.mDissolve = 1.000000;
	mat.illum = 2;
	mat.textureFile = "Textures\\" + texFile;
	frequency = 0;

	createPlane();

}


void Water::initProgram()
{
	srand(time(NULL));
	for (int i = 0; i < 8; ++i)
	{
		amplitude[i] = (rand() / RAND_MAX);
		amplitude[i] = 1.0 + amplitude[i] * (2.0 - 1.0);		//waveL = 0;
		speed[i] = (rand() / RAND_MAX);
		speed[i] = 10.0 + speed[i] * (20.0 - 10.0);
		direction[i] = vec2(rand() % 10 - 1, rand() % 10 - 1);

	}

	getShaderLocations();
	transferSettings();

	string l;
	//send each member of each light to its appropriate place in the shader
	l = "lights[0].";

	for (int i = 0; i < Lights.size(); i++)
	{
		l[7] = i + '0';

		Lights[i].init(program, l + "LightPosition", l + "LAmbient",
			l + "LDiffuse", l + "LSpecular");

		//send over light settings to the shader
		Lights[i].transferSettings(program);
	}

}

void Water::load(GLuint program)
{
	this->program = program;
	glUseProgram(program);

	//Gets shader data locations and send initial data over
	initProgram();

	cout << "Loading Water" << endl;
	//print out number data
	cout << "Number of verticies is " << vertices.size() << endl;
	cout << "Number of normals is " << normals.size() << endl;
	cout << "Number of textures is " << textures.size() << endl;
	cout << "Number of indicies is " << indicies.size() << endl << endl;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (hasTexture)
	{
		int size = (vertices.size() * sizeof(vec4));
		int sizeT = (textures.size() * sizeof(vec2));
		int sizeI = (indicies.size() * sizeof(GLint));

		// Create and initialize a buffer object
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		glBufferData(GL_ARRAY_BUFFER, size + size + sizeT,
			NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, size, size, &normals[0]);
		glBufferSubData(GL_ARRAY_BUFFER, size + size, sizeT, &textures[0]);

		// plumbing
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(size));

		GLuint texcoord = glGetAttribLocation(program, "texcoord");
		glEnableVertexAttribArray(texcoord);
		glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size + size));

		//Bind and configure element array buffer
		glGenBuffers(1, &iBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI,
		NULL, GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, &indicies[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		//Load Texture
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE3);
		texLoc = glGetUniformLocation(program, "tex");
		//Tell the shader that the texture is bounded to texture unit 3
		glUniform1i(texLoc, 3);
		glBindTexture(GL_TEXTURE_2D, tex);

		string fullTextureName = "Textures\\" + mat.textureFile;

		int width, height;
		unsigned char* image = SOIL_load_image(fullTextureName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//set active texture unit back to 0
		glActiveTexture(GL_TEXTURE0);

	}
	else
	{
		int size = (vertices.size() * sizeof(vec4));
		int sizeI = (indicies.size() * sizeof(GLuint));

		// Create and initialize a buffer object
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		glBufferData(GL_ARRAY_BUFFER, size + size,
			NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, &vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, size, size, &normals[0]);

		// plumbing
		GLuint vPosition = glGetAttribLocation(program, "vPosition");
		glEnableVertexAttribArray(vPosition);
		glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

		GLuint vNormal = glGetAttribLocation(program, "vNormal");
		glEnableVertexAttribArray(vNormal);
		glVertexAttribPointer(vNormal, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(size));

		//Bind and configure element array buffer
		glGenBuffers(1, &iBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, &indicies[0], GL_STATIC_DRAW);

	}

}

void Water::createPlane()
{
	vertices.resize(size * size);
	normals.resize(size * size);

	for (int z = 0; z < size; z++)
	{
		for (int x = 0; x < size; x++)
		{
			vertices[z * size + x] = vec4(x - (size / 2), 0.0, z - (size/2), 1.0);
			normals[z * size + x] = vec4(0.0, 1.0, 0.0, 0.0);
		}
	}

	//get the appropriate indicies
	for (int z = 0; z < size - 1; z++)
	{
		for (int x = 0; x < size; x++)
		{
			indicies.push_back(z * size + x);
			indicies.push_back((z + 1) * size + x);
		}

		//push the primitive restart index
		indicies.push_back(size * size);
	}
}

void Water::draw()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

	//transfer current data to the shader;
	transferSettings();

	if (hasTexture)
	{
		glBindTexture(GL_TEXTURE_2D, tex);
	}

	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(size * size);
	glDrawElements(GL_TRIANGLE_STRIP, indicies.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
	glDisable(GL_PRIMITIVE_RESTART);


}

void Water::getShaderLocations()
{
	ModelViewLoc = glGetUniformLocation(program, "model_view");
	MAmbientLoc = glGetUniformLocation(program, "MAmbient");
	MDiffuseLoc = glGetUniformLocation(program, "MDiffuse");
	MSpecularLoc = glGetUniformLocation(program, "MSpecular");
	ShininessLoc = glGetUniformLocation(program, "Shininess");
	textBoolLoc = glGetUniformLocation(program, "mapText");
	ProjectionViewLoc = glGetUniformLocation(program, "proj");
	deltaTimeLoc = glGetUniformLocation(program, "t");
	amplitudeLoc = glGetUniformLocation(program, "amplitude");
	speedLoc = glGetUniformLocation(program, "speed");
	directionLoc = glGetUniformLocation(program, "direction");
	frequencyLoc = glGetUniformLocation(program, "f");
}

void Water::transferSettings()
{
	mat4 model_view2 = model_view * translate * scale * rotate;
	glUniformMatrix4fv(ModelViewLoc, 1, GL_TRUE, model_view2);
	glUniformMatrix4fv(ProjectionViewLoc, 1, GL_TRUE, proj);
	glUniform3fv(MAmbientLoc, 1, mat.mAmbient);
	glUniform3fv(MDiffuseLoc, 1, mat.mDiffuse);
	glUniform3fv(MSpecularLoc, 1, mat.mSpecular);
	glUniform1f(ShininessLoc, mat.Shininess);
	glUniform1i(textBoolLoc, hasTexture);
	glUniform1f(deltaTimeLoc, glutGet(GLUT_ELAPSED_TIME) / 100);
	glUniform1fv(amplitudeLoc, 8, amplitude);
	glUniform1f(frequencyLoc, frequency);
	glUniform1fv(speedLoc, 8, speed);
	glUniform2fv(directionLoc, 8, direction[0]);

}

void Water::translateWater(vec3 t)
{
	translate = Translate(t);
}
void Water::scaleWater(vec3 s)
{
	scale = Scale(s);
}

void Water::rotateWater(vec3 r)
{
	rotate = RotateX(r.x) * RotateY(r.y) * RotateZ(r.z);
}

void Water::changeProgram(GLuint program)
{
	this->program = program;
}

GLuint Water::getProgram()
{
	return program;
}

void Water::setFrequency(float f)
{
	frequency = f;
}

void Water::setReflectionTex(GLuint ref)
{
	reflectTex = ref;
	glUseProgram(reflectTex);
	//Get location of reflectTex in shader and
	GLuint refLoc = glGetUniformLocation(program, "reflectTex");
	//tell the shader the texture is bounded to texture unit 4
	glUniform1i(refLoc, 4);

	glActiveTexture(GL_TEXTURE4);

	glBindTexture(GL_TEXTURE_CUBE_MAP, ref);

	//reset active texture unit to 0
	glActiveTexture(GL_TEXTURE0);

}

Water::~Water()
{
}
