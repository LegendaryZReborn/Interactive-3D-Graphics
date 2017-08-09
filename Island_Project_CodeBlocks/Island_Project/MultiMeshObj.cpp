#include "MultiMeshObj.h"
#include <fstream>


MultiMeshObj::MultiMeshObj()
{
}

MultiMeshObj::MultiMeshObj(string filename)
{
	numVertices = 0;
	numNormals = 0;
	numIndicies = 0;

	objFileName = filename;

	ctr_box = vec4(0, 0, 0, 1);
}
void MultiMeshObj::readData(string filename)
{
	ifstream infile;
	int indexOffsetV = 1, indexOffsetT = 1, indexOffsetN = 1;
	vector<vec4> vertexList;
	vector<vec2> textureList;
	vector<vec4> normalsList;
	vector<GLuint> textureIndexL;
	vector<GLuint> vertexIndexL;
	vector<GLuint> normalsIndexL;
	string data, data0;
	string materialFile, matName;
	GLfloat x, y, z;
	GLfloat xmin = 0, xmax = 0, zmin = 0, zmax = 0, ymin = 0, ymax = 0;
	GLfloat xminM = 0, xmaxM = 0, zminM = 0, zmaxM = 0, yminM = 0, ymaxM = 0;
	GLfloat xwidth, ywidth, zwidth;
	Material mat;

	//Read in material data 
	size_t pos = filename.find_last_of('.');	
	materialFile = filename.substr(0, pos);
	materialFile.append(".mtl");
	readMaterialData(materialFile);

	string fileNamePath = "Objects\\" + filename;
	infile.open(fileNamePath);
	if (infile)
	{
		infile >> data0;
		while (infile)
		{
			//subMesh begins here
			if (data0 == "o")
			{
				mapText = false;
				xmin = 0, xmax = 0, zmin = 0, zmax = 0, ymin = 0, ymax = 0;
				numVertices = 0;
				numNormals =  0;
				numIndicies = 0;
				numTextures = 0;
				
				indexOffsetN += normalsList.size();
				indexOffsetV += vertexList.size();
				indexOffsetT += textureList.size();

				//clear data in
				mappedVertices.clear();
				mappedNormalsList.clear();
				mappedTextures.clear();
				vertexList.clear();
				textureList.clear();
				normalsList.clear();
				textureIndexL.clear();
				vertexIndexL.clear();
				normalsIndexL.clear();

				infile >> data;
				//while its the same submesh
				while (data != "o" && infile)
				{
					if (data == "v")
					{
						infile >> x >> y >> z;
						vertexList.push_back(vec4(x, y, z, 1));

						//keep track of the max and min in each dimension
						if (x > xmax)
							xmax = x;
						else if (x < xmin)
							xmin = x;

						if (y > ymax)
							ymax = y;
						else if (y < ymin)
							ymin = y;

						//maxes and mins for the overall mesh
						if (z > zmaxM)
							zmaxM = z;
						else if (z < zmin)
							zminM = z;

						if (x > xmaxM)
							xmaxM = x;
						else if (x < xminM)
							xminM = x;

						if (y > ymaxM)
							ymaxM = y;
						else if (y < ymin)
							yminM = y;

						if (z > zmaxM)
							zmaxM = z;
						else if (z < zmin)
							zminM = z;

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
					else if (data == "usemtl")
					{
						infile >> matName;
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
									normalsIndexL.push_back(num - indexOffsetN);
								}
							}
							else
							{
								if (c == 0)
								{
									vertexIndexL.push_back(num - indexOffsetV);
									num = 0;
									c++;
								}
								else if (c == 1 )
								{
									//TextureIndices
									if(num != 0)
										textureIndexL.push_back(num - indexOffsetT);
									num = 0;
									c++;
								}
								else
								{
									normalsIndexL.push_back(num - indexOffsetN);
									num = 0;
									c = 0;
								}
							}
						}
					}
					infile >> data;
				} //end while

				//Do some mapping and other calculations for the current subMesh

				//if textureList is not empty, there is a texture to map
				if (textureList.size() != 0)
					mapText = true;

				//for the current sub mesh calculate its dimensions
				calculateMeshDimensions(xmin, xmax, zmin, zmax, ymin, ymax);

				//put vertices in mappedVerticies corresponding to the vertexIndexL
				for (int m = 0; m < vertexIndexL.size(); m++) {
					//add the vertex at index in vertexIndexL
					mappedVertices.push_back(vertexList[vertexIndexL[m]]);
					mappedNormalsList.push_back(normalsList[normalsIndexL[m]]);

					if (mapText)
						mappedTextures.push_back(textureList[textureIndexL[m]]);
				}

				
				numVertices = mappedVertices.size();
				numNormals = mappedNormalsList.size();
				numIndicies = vertexIndexL.size();
				numTextures = mappedTextures.size();

				//print out numbers
				cout << "Number of verticies is " << numVertices << endl;
				cout << "Number of normals is " << numNormals << endl;
				cout << "Number of indicies is " << numIndicies << endl;

				mat = materials.at(matName);

				//Create a object from subMesh data and add it to vector of objects
				Object subMesh(filename, numVertices, numNormals, numIndicies, numTextures, mapText, mappedVertices,
					mappedNormalsList, mappedTextures, box_max, ctr_box, mat);
				subMeshes.push_back(subMesh);

			}  //end if


			if (data == "o" && infile)
			{
				data0 = data;

			}
			else
			{
				infile >> data0;
			}

		}// end while

		infile.close();

		//for the overall mesh
		calculateMeshDimensions(xminM, xmaxM, zminM, zmaxM, yminM, ymaxM);

		
	}
	else
	{
		cout << "File could not be opened!" << endl;
	}
}

void MultiMeshObj::readMaterialData(string filename)
{
	ifstream infile;
	string data, data2, matName;
	string fileNamePath;
	Material mat;


	fileNamePath = "Objects\\" + filename;

	infile.open(fileNamePath);
	if (infile)
	{
		infile >> data;
		while (infile)
		{
			if(data == "newmtl")
			{
				infile >> data2;
				matName = data2;
				while (data2 != "newmtl" && infile) 
				{
					if (data2 == "Ns")
					{
						infile >> mat.Shininess;
					}
					else if (data2 == "Ka")
					{
						infile >> mat.mAmbient.x >> mat.mAmbient.y >> mat.mAmbient.z;
					}
					else if (data2 == "Kd")
					{
						infile >> mat.mDiffuse.x >> mat.mDiffuse.y >> mat.mDiffuse.z;
					}
					else if (data2 == "Ks")
					{
						infile >> mat.mSpecular.x >> mat.mSpecular.y >> mat.mSpecular.z;
					}
					else if (data2 == "Ni")
					{
						infile >> mat.mOpticalDensity;
					}
					else if (data2 == "d")
					{
						infile >> mat.mDissolve;
					}
					else if (data2 == "illum")
					{
						infile >> mat.illum;
					}
					else if (data2 == "map_Kd")
					{
						mapText = true;
						infile >> mat.textureFile;

						
						size_t pos = mat.textureFile.find_last_of("\\");

						if(pos != string::npos)
							mat.textureFile = mat.textureFile.substr(pos + 1);
					}

					infile >> data2;
				} //endwhile

				materials.insert(pair<string, Material>(matName, mat));

			}//end if
			
			
			if(data2 != "newmtl")
				infile >> data;
		} // end while
	}
	else
	{
		cout << "Material file could not be opened!" << endl;
	}
}

void MultiMeshObj::Load(GLuint program)
{
	readData(objFileName);
	
	if (!subMeshes.empty())
	{
		for (int i = 0; i < subMeshes.size() ; i++)
		{
			subMeshes[i].Load(program);
		}
	}
}
void MultiMeshObj::Draw()
{
	if (!subMeshes.empty())
	{
		for (int i = 0; i < subMeshes.size(); i++)
		{

			subMeshes[i].Draw();


		}
	}
}

vec4 MultiMeshObj::provideBoxCenter()
{
	return ctr_box;
}

GLfloat  MultiMeshObj::provideBoxMax()
{
	return box_max;
}

void MultiMeshObj::calculateMeshDimensions(GLfloat xmin, GLfloat xmax, GLfloat zmin, GLfloat zmax ,
	GLfloat ymin , GLfloat ymax)
{
	GLfloat xwidth, ywidth, zwidth;
	//calculate the box_max width
	xwidth = xmax - xmin;
	ywidth = ymax - ymin;
	zwidth = zmax - zmin;

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
	//ctr_box = vec4(xmin + (xwidth) / 2.0, ymin + (ywidth) / 2.0, zmin + (zwidth / 2.0), 1);
	ctr_box = vec4(0, 0, 0, 1);
}

void MultiMeshObj::translateObj(vec3 t)
{
	if (!subMeshes.empty())
	{
		for (int i = 0; i < subMeshes.size(); i++)
		{

			subMeshes[i].translateObj(t);

		}
	}
}
void MultiMeshObj::scaleObj(vec3 s)
{
	if (!subMeshes.empty())
	{
		for (int i = 0; i < subMeshes.size(); i++)
		{

			subMeshes[i].scaleObj(s);

		}
	}
}
string MultiMeshObj::getFileName()
{
	return objFileName;
}
GLuint MultiMeshObj::getProgram(int subIndex)
{
	return subMeshes[subIndex].program;
}


MultiMeshObj::~MultiMeshObj()
{
}
