
//Cavaughn Browne
#include "Terrain.h"
#include <SOIL.h>
#include <fstream>
#include "DirLight.h"
#include "PointLight.h"

extern mat4 model_view;
extern vector<DirLight> Lights;
extern mat4 proj;


Terrain::Terrain()
{
	numVertices = 0;
	numNormals = 0;
	numIndicies = 0;
	objFileName = "";
	ctr_box = vec4(0, 0, 0, 1);

	//set the materials for the terrain
	mat.Shininess = 96.0;
	mat.mAmbient = vec3(0.5, 0.5, 0.5);
	mat.mDiffuse = vec3(1.0, 1.0, 1.0);
	mat.mSpecular = vec3(0.500000, 0.500000, 0.500000);
	mat.mOpticalDensity = 1.000000;
	mat.mDissolve = 1.000000;
	mat.illum = 2;

}

Terrain::Terrain(string filename, bool texture)
{
	numVertices = 0;
	numNormals = 0;
	numIndicies = 0;

	objFileName = filename;

	ctr_box = vec4(0, 0, 0, 1);
	size = 512;
	mapText = texture;

	init(objFileName);

	mat.Shininess = 60.0;
	mat.mAmbient = vec3(0.0, 0.0, 0.0);
	mat.mDiffuse = vec3(1.0, 1.0, 1.0);
	mat.mSpecular = vec3(0.000000, 0.00000, 0.00000);
	mat.mOpticalDensity = 1.000000;
	mat.mDissolve = 1.000000;
	mat.illum = 2;
	size_t pos = filename.find('.');
	mat.textureFile = filename.substr(0, pos);
	mat.textureFile.append(".jpg");

}

Terrain::Terrain(string filename, bool texture, Material mat1, int size)
{
	numVertices = 0;
	numNormals = 0;
	numIndicies = 0;
	objFileName = filename;
	ctr_box = vec4(0, 0, 0, 1);
	this->size = size;
	mapText = texture;

	//Initializes the terrain
	init(objFileName);

	mat = mat1;
}
void Terrain::programInit()
{
	getShaderLocations();
	transferSettings();

}
void Terrain::Load(GLuint program)
{
	this->program = program;
	glUseProgram(program);
	programInit();

	cout << "Loading Terrain" << objFileName << endl;
	//print out number data
	cout << "Number of verticies is " << numVertices << endl;
	cout << "Number of normals is " << numNormals << endl;
	cout << "Number of indicies is " << numIndicies << endl << endl;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	if (mapText)
	{
		// Create and initialize a buffer object
		int size = (numVertices * sizeof(vec4));
		int sizeT = (numTextures * sizeof(vec2));
		int sizeI = numIndicies * sizeof(GLuint);

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

		//Load Texture
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE1);
		texLoc = glGetUniformLocation(program, "tex");
		glUniform1i(texLoc, 1);
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

		//detailTexture
		string detailTextureName = "Textures\\";
		detailTextureName.append("detail.jpg");

		glGenTextures(1, &detailTex);
		dTexLoc = glGetUniformLocation(program, "detailTex");
		glUniform1i(dTexLoc, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, detailTex);

		image = SOIL_load_image(detailTextureName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//detailTexture2
		detailTextureName = "Textures\\";
		detailTextureName.append("detail2.jpg");

		glGenTextures(1, &detailTex2);
		dTexLoc = glGetUniformLocation(program, "detailTex2");
		glUniform1i(dTexLoc, 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, detailTex2);

		image = SOIL_load_image(detailTextureName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glActiveTexture(GL_TEXTURE0);

		//Bind and configure element array buffer
		glGenBuffers(1, &iBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, &indexList[0], GL_STATIC_DRAW);

	}
	else
	{
		int size = (numVertices * sizeof(vec4));
		int sizeI = numIndicies * sizeof(GLuint);

		//color and vertices go in the same buffer
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

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, &indexList[0], GL_STATIC_DRAW);
	}

}

//Parses and reads in data from an object file
void Terrain::init(string filename)
{
	int width, height;
	float ruggedFactor;
	vector<vector <vec4>> QNormals[2];
	vector<vector<vec4>> vFinalNormals;
	vector<vector<vec3>> vVertexData;
	vector<vector<vec2>> textureData;

	string fileNamePath = "Terrains\\" + filename;
	GLfloat h;

	//Load the image
	unsigned char* image = SOIL_load_image(fileNamePath.c_str(), &width, &height, 0, SOIL_LOAD_L);

	if (image != NULL)
	{
		vertices.resize(height * width);
		textures.resize(height * width);

		//Resize the grid to hold the heights from the heightmap
		heights.resize(height);
		for (int zA = 0; zA < height; zA++)
		{
			heights[zA].resize(width);
		}

		ruggedFactor = 240.0/2;

		for (int i = 0; i < height; i++)
		{
			for(int j = 0; j < width; j++)
			{
				//store hieght map heights
				heights[i][j] = image[i * height + j] / 255.0;

				h = heights[i][j];

				GLfloat z = (i / (GLfloat)height) * size;
				GLfloat x = (j / (GLfloat)width) * size;

				textures[i * height + j] = vec2(x / size, z / size);

				//store vertices - for each row i and each column j
				vertices[i * height + j] = vec4(x - (size / 2), h * ruggedFactor, z - (size / 2), 1.0);

			}
		}

		//Free image data memory;
		SOIL_free_image_data(image);

		//resize vector of vectors to hold  normals for each triangle in a quad
		for (int i = 0; i < 2; i++)
		{
			QNormals[i] = vector< vector<vec4> >(width - 1, vector<vec4>(height - 1));
		}

		//calculate the normals for each triangle
		//Triangle0 is the lower left triangle in the quad while Triangle1 is the upper right triangle
		for (int row = 0; row < height - 1; row++)
		{
			for (int col = 0; col < width - 1; col++)
			{
				vec4 Triangle0Norm = cross(vertices[(row + 1) * height + col] - vertices[row * height + col],
					vertices[(row + 1) * height + (col + 1)] - vertices[row * height + col]);
				Triangle0Norm.w = 0;

				vec4 Triangle1Norm = cross(vertices[(row + 1) * height + (col + 1)] - vertices[row * height + col],
					vertices[row * height + (col + 1)] - vertices[row * height + col]);
				Triangle1Norm.w = 0;


				QNormals[0][row][col] = normalize(Triangle0Norm);
				QNormals[1][row][col] = normalize(Triangle1Norm);
			}
		}


		normals.resize(height * width);
		//Now time to calculate the vertex normals
		vector< vector<vec4> > vFinalNormals = vector< vector<vec4> >(width, vector<vec4>(height));

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				// Now we wanna calculate final normal for [i][j] vertex. We will have a look at all triangles this vertex is part of, and then we will make average vector
				// of all adjacent triangles' normals

				vec4 vFinalNormal = vec4(0.0f, 0.0f, 0.0f, 0.0f);

				// Look for upper-left triangles
				if (j != 0 && i != 0)
					for (int k = 0; k < 2; k++)
						vFinalNormal += QNormals[k][i - 1][j - 1];
				// Look for upper-right triangles
				if (i != 0 && j != width - 1)
					vFinalNormal += QNormals[0][i - 1][j];
				// Look for bottom-right triangles
				if (i != height - 1 && j != width - 1)
					for (int k = 0; k < 2; k++)
						vFinalNormal += QNormals[k][i][j];
				// Look for bottom-left triangles
				if (i != height - 1 && j != 0)
					vFinalNormal += QNormals[1][i][j - 1];
			//	vFinalNormal *= -1;
				vFinalNormal = normalize(vFinalNormal);
			///	vFinalNormal = vec4(0.0, 1.0, 0.0, 0.0);
				//vFinalNormal = vFinalNormal / vFinalNormal.w;
				vFinalNormals[i][j] = vFinalNormal; // Store final normal of j-th vertex in i-th row
				normals[i * height + j] = vFinalNormals[i][j];

			}
		}

		//Fill up index List with the appropriate indicies in order
		for (int i = 0; i < height - 1; i++)
		{
			for (int j = 0; j < width; j++)
			{
				indexList.push_back(i * size + j);
				indexList.push_back((i + 1) * size + j);
			}

			//push the primitive restart index
			indexList.push_back(height * width);
		}


		numVertices = vertices.size();
		numNormals = normals.size();
		numTextures = textures.size();
		numIndicies = indexList.size();

		iPrimitiveRestartIndex = height * width;

	}
	else
	{
		cout << "Heightmap not be opened!" << endl;
	}

}

void Terrain::Draw()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);

	getShaderLocations();

	//send over material settings to the shader
	transferSettings();
    transferLights();

    if (mapText)
	{
		//Set bool in shader to true because a texture is being used
		glUniform1ui(textBoolLoc, true);
		glBindTexture(GL_TEXTURE_2D, tex);
		glBindTexture(GL_TEXTURE_2D, detailTex);
		glBindTexture(GL_TEXTURE_2D, detailTex2);
	}
	else
	{
		glUniform1ui(textBoolLoc, false);

	}

	glEnable(GL_PRIMITIVE_RESTART);
	 glPrimitiveRestartIndex(iPrimitiveRestartIndex);

	glDrawElements(GL_TRIANGLE_STRIP, numIndicies, GL_UNSIGNED_INT,BUFFER_OFFSET(0));

	//glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glDisable(GL_PRIMITIVE_RESTART);

}

vector<vec4> Terrain::getNormals()
{
	return normals;
}

vec4 Terrain::provideBoxCenter()
{
	return ctr_box;
}

GLfloat Terrain::provideBoxMax()
{
	return box_max;
}

void Terrain::translateTerrain(vec3 t)
{
	translate = Translate(t);
}

void Terrain::scaleTerrain(vec3 s)
{
	scale = Scale(s);
}

void Terrain::rotateTerrain(GLfloat xAxis, GLfloat yAxis, GLfloat zAxis)
{
	rotate = RotateX(xAxis) * RotateY(yAxis) * RotateZ(zAxis);
}


void Terrain::getShaderLocations()
{
	ModelViewLoc = glGetUniformLocation(program, "model_view");
	MAmbientLoc = glGetUniformLocation(program, "MAmbient");
	MDiffuseLoc = glGetUniformLocation(program, "MDiffuse");
	MSpecularLoc = glGetUniformLocation(program, "MSpecular");
	ShininessLoc = glGetUniformLocation(program, "Shininess");
	textBoolLoc = glGetUniformLocation(program, "mapText");
	ProjectionViewLoc = glGetUniformLocation(program, "proj");
	deltaTimeLoc = glGetUniformLocation(program, "t");

}

void Terrain::transferSettings()
{
	mat4 model_view2 = model_view * translate * scale * rotate;
	glUniformMatrix4fv(ModelViewLoc, 1, GL_TRUE, model_view2);
	glUniformMatrix4fv(ProjectionViewLoc, 1, GL_TRUE, proj);
	glUniform3fv(MAmbientLoc, 1, mat.mAmbient);
	glUniform3fv(MDiffuseLoc, 1, mat.mDiffuse);
	glUniform3fv(MSpecularLoc, 1, mat.mSpecular);
	glUniform1f(ShininessLoc, mat.Shininess);
	glUniform1i(textBoolLoc, mapText);
	glUniform1f(deltaTimeLoc, glutGet(GLUT_ELAPSED_TIME) / 200);
}

void Terrain::transferLights()
{
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

Terrain::~Terrain()
{
}
