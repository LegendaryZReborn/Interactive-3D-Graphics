//Cavaughn Browne
#include "Object.h"
#include <SOIL.h>
#include <fstream>
#include "DirLight.h"
#include "PointLight.h"

extern mat4 model_view;
extern vector<DirLight> Lights;
//Lights in the temple
extern vector<PointLight> ptempleLights;
extern mat4 proj;

Object::Object()
{
	numVertices = 0;
	numNormals = 0;
	numIndicies = 0;
	objFileName = "";
	ctr_box = vec4(0, 0, 0, 1);


}

Object::Object(string filename, int numV, int numNorm, int numInd, int numText, bool texture, vector<vec4>& mVerts,
	vector<vec4>& mNormL, vector<vec2>& mTex, GLfloat bMax, vec4 ctrBox, Material mat1)
{

	objFileName = filename;

	ctr_box = vec4(0, 0, 0, 1);
	mappedVertices = mVerts;
	mappedNormalsList = mNormL;
	mappedTextures = mTex;

	numVertices = numV;
	numNormals = numNorm;
	numIndicies = numInd;
	numTextures = numText;
	mapText = texture;

	box_max = bMax;
	ctr_box = ctrBox;

	mat = mat1;
}

Object::Object(string filename)
{
	numVertices = 0;
	numNormals = 0;
	numIndicies = 0;

	objFileName = filename;

	ctr_box = vec4(0, 0, 0, 1);

	readData(objFileName);

}

void Object::Load(GLuint program)
{
	this->program = program;
	glUseProgram(program);

	Init();


	cout << "Loading Object " << objFileName << endl;
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

		//Load Texture
		glGenTextures(1, &tex);
		texLoc = glGetUniformLocation(program, "tex");
		glUniform1i(texLoc, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

		string fullTextureName = "Textures\\" + mat.textureFile;

		int width, height;
		unsigned char* image = SOIL_load_image(fullTextureName.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		//set texture parameters
		//GL_CLAMP_TO_EDGE
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

	}
	else
	{
		// Create and initialize a buffer object
		int size = (numVertices * sizeof(vec4));

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

	}

	//unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);


}

void Object::Init()
{
	getShaderLocations();
	transferSettings();
}

void Object::Draw()
{
	glUseProgram(program);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	getShaderLocations();

	//send over light settings
	transferLights();
	//send over material settings and tex to the shader
	transferSettings();

	if (mapText)
	{
		//texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);

	}

	glDrawArrays(GL_TRIANGLES, 0, numVertices);

	//unbind texture and buffer
	if (mapText)
		//glBindTexture(GL_TEXTURE_2D, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Object::readMaterialData(string filename)
{
	ifstream infile;
	string data;
	string fileNamePath;

	fileNamePath = "Objects\\" + filename;

	infile.open(fileNamePath);
	if (infile)
	{
		infile >> data;
		while (infile)
		{
			if (data == "Ns")
			{
				infile >> mat.Shininess;
			}
			else if (data == "Ka")
			{
				infile >> mat.mAmbient.x >> mat.mAmbient.y >> mat.mAmbient.z;
			}
			else if (data == "Kd")
			{
				infile >> mat.mDiffuse.x >> mat.mDiffuse.y >> mat.mDiffuse.z;
			}
			else if (data == "Ks")
			{
				infile >> mat.mSpecular.x >> mat.mSpecular.y >> mat.mSpecular.z;
			}
			else if (data == "Ni")
			{
				infile >> mat.mOpticalDensity;
			}
			else if (data == "d")
			{
				infile >> mat.mDissolve;
			}
			else if (data == "illum")
			{
				infile >> mat.illum;
			}
			else if (data == "map_Kd")
			{
				mapText = true;
				infile.ignore();
				getline(infile, mat.textureFile);


				size_t pos = mat.textureFile.find_last_of("\\");

				if (pos != string::npos)
					mat.textureFile = mat.textureFile.substr(pos + 1);
			}

			infile >> data;
		}
	}
	else
	{
		cout << "Material file could not be opened!" << endl;
	}
}

//Parses and reads in data from an object file
void Object::readData(string filename)
{
	ifstream infile;
	vector<vec4> vertexList;
	vector<vec2> textureList;
	vector<vec4> normalsList;
	vector<GLuint> textureIndexL;
	vector<GLuint> vertexIndexL;
	vector<GLuint> normalsIndexL;
	string data;
	string materialFile;
	GLfloat x, y, z;
	GLfloat xmin = FLT_MAX, xmax = -FLT_MAX, zmin = FLT_MAX, zmax = -FLT_MAX, ymin = FLT_MAX, ymax = -FLT_MAX;
	GLfloat xwidth, ywidth, zwidth;

	string fileNamePath = "Objects\\" + filename;
	infile.open(fileNamePath);
	if (infile)
	{
		infile >> data;
		while (infile)
		{
			if (data == "v")
			{
				infile >> x >> y >> z;

				vertexList.push_back(vec4(x, y, z, 1));

				//keep track of the max and min in each dimension
				xmin = min(x, xmin);
				ymin = min(y, ymin);
				zmin = min(z, zmin);
				xmax = max(x, xmax);
				ymax = max(y, ymax);
				zmax = max(z, zmax);

			}
			else if (data == "vn")
			{
				infile >> x >> y >> z;
				normalsList.push_back(vec4(x, y, z, 0));
			}
			else if (data == "vt")
			{
				infile >> x >> y;
				textureList.push_back(vec2(x, y));

			}
			else if (data == "f")
			{
				infile.ignore();
				getline(infile, data);
				int num = 0, c = 0;
				for (int i = 0; i < data.length(); i++)
				{
					if (isdigit(data[i]))
					{
						num = num * 10 + (data[i] - '0');

						if (i == data.length() - 1)
						{
							normalsIndexL.push_back(num - 1);
						}
					}
					else
					{
						if (c == 0)
						{
							vertexIndexL.push_back(num - 1);
							num = 0;
							c++;
						}
						else if (c == 1)
						{
							//TextureIndices
							textureIndexL.push_back(num - 1);
							num = 0;
							c++;
						}
						else
						{
							normalsIndexL.push_back(num - 1);
							num = 0;
							c = 0;
						}
					}
				}
			}

			infile >> data;
		}


		infile.close();

		//if textureList is not empty, there is a texture to map
		if (textureList.size() != 0)
			mapText = true;

		//calculate the box_max width
		xwidth = xmax - xmin;
		ywidth = ymax - ymin;
		zwidth = zmax - zmin;

		//Store the max and min points
		boxMaxP = vec4(xmax, ymax, zmax, 1.0);
		boxMinP = vec4(xmin, ymin, zmin, 1.0);

		//create 8 points of bounding box 
		//starting with back plane
		bBoxPoints.push_back(boxMinP);
		bBoxPoints.push_back(vec4(boxMinP.x, boxMinP.y + ywidth, boxMinP.z, 1.0));
		bBoxPoints.push_back(vec4(boxMinP.x + xwidth, boxMinP.y + ywidth, boxMinP.z, 1.0));
		bBoxPoints.push_back(vec4(boxMinP.x + xwidth, boxMinP.y, boxMinP.z, 1.0));

		//starting with front plane
		bBoxPoints.push_back(vec4(boxMaxP.x, boxMaxP.y - ywidth, boxMaxP.z, 1.0));
		bBoxPoints.push_back(vec4(boxMaxP.x - xwidth, boxMaxP.y - ywidth, boxMaxP.z, 1.0));
		bBoxPoints.push_back(vec4(boxMaxP.x - xwidth, boxMaxP.y, boxMaxP.z, 1.0));
		bBoxPoints.push_back(boxMaxP);




		//find the maximum with among all three dimensions
		if (xwidth >= ywidth && xwidth >= zwidth)
		{
			box_max = xwidth;
		}
		else if (ywidth >= xwidth && ywidth >= zwidth)
		{
			box_max = ywidth;
		}
		else
		{
			box_max = zwidth;
		}

		//calculate center
		ctr_box = vec4(xmin + (xwidth) / 2.0, ymin + (ywidth) / 2.0, zmin + (zwidth / 2.0), 1);


		//put vertices in mappedVerticies corresponding to the vertexIndexL
		for (int m = 0; m < vertexIndexL.size(); m++) {
			//add the vertex at index in vertexIndexL
			mappedVertices.push_back(vertexList[vertexIndexL[m]]);
			mappedNormalsList.push_back(normalsList[normalsIndexL[m]]);

			if (mapText)
				mappedTextures.push_back(textureList[textureIndexL[m]]);
		}

		numVertices = mappedVertices.size();
		numNormals = normalsList.size();
		numIndicies = vertexIndexL.size();
		numTextures = mappedTextures.size();



		//Read in materials
		size_t pos = filename.find_last_of('.');
		materialFile = filename.substr(0, pos);
		materialFile.append(".mtl");
		readMaterialData(materialFile);
	}
	else
	{
		cout << "File could not be opened!" << endl;
	}
}

vector<vec4> Object::getVertices()
{
	return mappedVertices;
}

vec4 Object::provideBoxCenter()
{
	return translate * scale * rotate * ctr_box;
}

GLfloat Object::provideBoxMax()
{
	return box_max;
}

void Object::getShaderLocations()
{
	ModelViewLoc = glGetUniformLocation(program, "model_view");
	ProjectionViewLoc = glGetUniformLocation(program, "proj");
	MAmbientLoc = glGetUniformLocation(program, "MAmbient");
	MDiffuseLoc = glGetUniformLocation(program, "MDiffuse");
	MSpecularLoc = glGetUniformLocation(program, "MSpecular");
	ShininessLoc = glGetUniformLocation(program, "Shininess");
	textBoolLoc = glGetUniformLocation(program, "mapText");
	deltaTimeLoc = glGetUniformLocation(program, "t");

}

void Object::transferSettings()
{
	mat4 model_view2 = model_view * translate * scale * rotate;
	glUniformMatrix4fv(ModelViewLoc, 1, GL_TRUE, model_view2);
	glUniformMatrix4fv(ProjectionViewLoc, 1, GL_TRUE, proj);
	glUniform3fv(MAmbientLoc, 1, mat.mAmbient);
	glUniform3fv(MDiffuseLoc, 1, mat.mDiffuse);
	glUniform3fv(MSpecularLoc, 1, mat.mSpecular);
	glUniform1f(ShininessLoc, mat.Shininess);
	glUniform1i(textBoolLoc, mapText);
	glUniform1f(deltaTimeLoc, glutGet(GLUT_ELAPSED_TIME) / 1000);
}

void Object::translateObj(vec3 t)
{
	translate = Translate(t);

}

void Object::scaleObj(vec3 s)
{
	scale = Scale(s);

}

void Object::rotateObj(GLfloat xAxis, GLfloat yAxis, GLfloat zAxis)
{
	rotate = RotateX(xAxis) * RotateY(yAxis) * RotateZ(zAxis);

}

vec4 Object::provideMinPoint()
{
	return translate * scale * rotate * boxMinP;

}
vec4 Object::provideMaxPoint()
{
	return translate * scale * rotate *boxMaxP;
}

vector<vec4> Object::provideAABB()
{
	vector<vec4> newAABB;

	for (auto x : bBoxPoints)
	{
		newAABB.push_back(translate * scale * rotate *x);
	}

	return newAABB;
}

void Object::transferLights()
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

	l = "pLight[0].";

	for (int i = 0; i < ptempleLights.size(); i++)
	{
		l[7] = i + '0';

		ptempleLights[i].init(program, l + "LightPosition", l + "LAmbient",
			l + "LDiffuse", l + "LSpecular", l + "constant", l + "linear", l + "quadratic");

		//send over light settings to the shader
		ptempleLights[i].transferSettings(program);
	}


}

Object::~Object()
{
}
