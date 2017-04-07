#include "Terrain.h"

extern mat4 model_view;

//Cavaughn Browne
#include "Terrain.h"
#include <SOIL.h>
#include <fstream>



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

	mat.Shininess = 96;
	mat.mAmbient = vec3(1.0, 1.0, 1.0);
	mat.mDiffuse = vec3(1.0, 1.0, 1.0);
	mat.mSpecular = vec3(0.500000, 0.500000, 0.500000);
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
	//size_t pos = filename.find('.');
	//textureFile = filename.substr(0, pos);
	//textureFile.append(".jpg");
	
	ctr_box = vec4(0, 0, 0, 1);

	this->size = size;
	mapText = texture;

	mat = mat1;


	init(objFileName);


}

void Terrain::Load(GLuint& program)
{
	glUseProgram(program);

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

		//color and vertices go in the same buffer
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);

		glBufferData(GL_ARRAY_BUFFER, size + size + sizeT,
			NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, &mappedVertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, size, size, &mappedNormalsList[0]);
		glBufferSubData(GL_ARRAY_BUFFER, size + size, sizeT, &mappedTextures[0]);

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

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, &indexList[0], GL_STATIC_DRAW);

		//Load Texture
		GLuint  texLoc;
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
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

		glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

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
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, &mappedVertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, size, size, &mappedNormalsList[0]);

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

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI,
			NULL, GL_STATIC_DRAW);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeI, &indexList[0], GL_STATIC_DRAW);

		//GLuint texcoord = glGetAttribLocation(program, "texcoord");
		//glEnableVertexAttribArray(texcoord);
		//glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(size + size));

	}

	//unbind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Terrain::Draw(GLuint& program)
{
	glUseProgram(program);
	glBindVertexArray(vao);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	getShaderLocations(program);

	//send over material settings to the shader
	transferSettings();

	if (mapText)
	{
		//texture
		glUniform1ui(textBoolLoc, true);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		texLoc = glGetUniformLocation(program, "tex");
		glUniform1ui(texLoc, tex);
	}
	else
	{
		//glUniform1ui(textBoolLoc, false);

	}
	//glPrimitiveRestartIndex(iPrimitiveRestartIndex);
	//glDrawElements(GL_TRIANGLE_STRIP, numIndicies, GL_UNSIGNED_INT, 0);

	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	//unbind texture and buffer
	if (mapText)
		glBindTexture(GL_TEXTURE_2D, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


//Parses and reads in data from an object file
void Terrain::init(string filename)
{
	struct VD3
	{
		VD3()
		{

		}
		VD3(vec3 vc, int i)
		{
			v.x = vc.x;
			v.y = vc.y;
			v.z = vc.z;
			index = i;
		}

		vec3 v;
		int index;
	};


	int width, height;
	float ruggedFactor;
	vector<vector <vec4>> QNormals[2];
	vector<vector<vec4>> vFinalNormals;
	vector<vector<VD3>> vVertexData;	
	vector<vector<vec2>> textureData;
	vector <vec4> norms;
	
	string fileNamePath = "Terrains\\" + filename;
	GLfloat h;

	//Load the image
	unsigned char* image = SOIL_load_image(fileNamePath.c_str(), &width, &height, 0, SOIL_LOAD_L);

	if (image != NULL)
	{
		//Resize the grid representing the heights of the vertices
		heights.resize(height);
		vVertexData.resize(height);
		textureData.resize(height);
		for (int zA = 0; zA < height; zA++)
		{
			heights[zA].resize(width);
			vVertexData[zA].resize(width);
			textureData[zA].resize(width);
		}

		ruggedFactor = 255.0/2;

		for (int i = 0; i < height; i++)
		{
			//norms.clear();
			for(int j = 0; j < width; j++)
			{ 
				heights[i][j] = image[i * height + j] / 255.0;

				h = heights[i][j];

				GLfloat z = (i / (GLfloat)height) * size;
				GLfloat x = (j / (GLfloat)width) * size;

				textureData[i][j] = vec2(x / size, z / size);
				
				
				vVertexData[i][j] = VD3(vec3(x - (size / 2), h * ruggedFactor , z - (size / 2)), i * width + j);
				//temporary normal
				norms.push_back(vec4(0.0, 1.0, 0.0, 0.0));
			}
			//vFinalNormals.push_back(norms);
		}	

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
				vec4 Triangle0Norm = cross(vVertexData[row + 1][col].v - vVertexData[row][col].v,
					vVertexData[row + 1][col + 1].v - vVertexData[row][col].v);
				Triangle0Norm.w = 0;

				vec4 Triangle1Norm = cross(vVertexData[row + 1][col + 1].v - vVertexData[row][col].v,
					vVertexData[row][col + 1].v - vVertexData[row][col].v);
				Triangle1Norm.w = 0;


				QNormals[0][row][col] = normalize(Triangle0Norm);
				QNormals[1][row][col] = normalize(Triangle1Norm);
			}
		}
			


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


			}
		}

		//Map all the data to their appropriate indicies for glDrawArrays
		for (int row = 0; row < height - 1; row++)
		{
			for (int col = 0; col < width - 1; col++)
			{
				//map first triangle in quad
				mappedVertices.push_back(vVertexData[row][col].v);
				heights2.push_back(vVertexData[row][col].v.y);
				mappedNormalsList.push_back(vFinalNormals[row][col]);
				mappedTextures.push_back(textureData[row][col]);
				indexList.push_back(vVertexData[row][col].index);
				
				mappedVertices.push_back(vVertexData[row + 1][col].v);
				heights2.push_back(vVertexData[row + 1][col].v.y);
				mappedNormalsList.push_back(vFinalNormals[row + 1][col]);
				mappedTextures.push_back(textureData[row + 1][col]);

				indexList.push_back(vVertexData[row + 1][col].index);

				mappedVertices.push_back(vVertexData[row + 1][col + 1].v);
				heights2.push_back(vVertexData[row + 1][col + 1].v.y);
				mappedNormalsList.push_back(vFinalNormals[row + 1][col + 1]);
				mappedTextures.push_back(textureData[row+1][col + 1]);

				indexList.push_back(vVertexData[row + 1][col + 1].index);
				

				//map second triangle in quad
				mappedVertices.push_back(vVertexData[row][col].v);
				heights2.push_back(vVertexData[row][col].v.y);
				mappedNormalsList.push_back(vFinalNormals[row][col]);
				mappedTextures.push_back(textureData[row][col]);
				indexList.push_back(vVertexData[row][col].index);


				mappedVertices.push_back(vVertexData[row + 1][col + 1].v);
				heights2.push_back(vVertexData[row + 1][col + 1].v.y);
				mappedNormalsList.push_back(vFinalNormals[row + 1][col + 1]);
				mappedTextures.push_back(textureData[row + 1][col + 1]);
				indexList.push_back(vVertexData[row + 1][col + 1].index);

				mappedVertices.push_back(vVertexData[row ][col + 1].v);
				heights2.push_back(vVertexData[row][col + 1].v.y);
				mappedNormalsList.push_back(vFinalNormals[row][col + 1]);
				mappedTextures.push_back(textureData[row][col + 1]);
				indexList.push_back(vVertexData[row][col + 1].index);

			}

		}

		numVertices = mappedVertices.size();
		numNormals = mappedNormalsList.size();
		numTextures = mappedTextures.size();
		numIndicies = indexList.size();

	}
	else
	{
		cout << "Heightmap not be opened!" << endl;
	}


		
}

vector<GLfloat> Terrain::getHeigtMap()
{
	return heights2;
}

vector<vec4> Terrain::getNormals()
{
	return mappedNormalsList;
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


void Terrain::getShaderLocations(GLuint& program)
{
	glUseProgram(program);

	ModelViewLoc = glGetUniformLocation(program, "model_view");
	MAmbientLoc = glGetUniformLocation(program, "MAmbient");
	MDiffuseLoc = glGetUniformLocation(program, "MDiffuse");
	MSpecularLoc = glGetUniformLocation(program, "MSpecular");
	ShininessLoc = glGetUniformLocation(program, "Shininess");
	textBoolLoc = glGetUniformLocation(program, "mapText");


}

void Terrain::transferSettings()
{
	mat4 model_view2 = model_view * translate * scale * rotate;
	glUniformMatrix4fv(ModelViewLoc, 1, GL_TRUE, model_view2);
	glUniform3fv(MAmbientLoc, 1, mat.mAmbient);
	glUniform3fv(MDiffuseLoc, 1, mat.mDiffuse);
	glUniform3fv(MSpecularLoc, 1, mat.mSpecular);
	glUniform1f(ShininessLoc, mat.Shininess);
	glUniform1i(textBoolLoc, mapText);
}

Terrain::~Terrain()
{
}
