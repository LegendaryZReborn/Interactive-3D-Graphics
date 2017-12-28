#include "Temple.h"
#include "PointLight.h"

extern bool checkFrustrum(vector<vec4> bBPoints);
extern vector<PointLight> ptempleLights;

Temple::Temple()
{
	string n;

	datPath = "Files\\placement.dat";
	fileNamePath = "C:\\Users\\Cavaughn\\Documents\\Visual Studio 2015";
	fileNamePath += "\\Projects\\Graphics\\Island Project\\GraphicsProject3_ReadModels\\Files\\";
	fileNamePath += "TempleFileNames.txt";

	//Reads the file containg the object names, locations etc
	readPlacementFile();


	//Add point lights at the specified locations
	for (auto l : locations["Light"])
	{
		ptempleLights.push_back(PointLight(l, vec3(1.0, 1.0, 1.0),
			vec3(1.0, 1.0, 1.0), vec3(1.0, 1.0, 1.0), 0.00001, 0.0009, 0.05));

	}


	bool duplicate;
	for (auto m : objNames)
	{
			n = "Temple\\" + m + ".obj";

			//Make sure a duplicate isn't added to the list
			duplicate = false;
			for (int x = 0; x < objects.size() && duplicate == false; x++)
			{
				if (objects[x].objFileName == n)
					duplicate = true;
			}

			if(!duplicate)
				objects.push_back(Object(n));

	}


}

void Temple::load(GLuint program)
{
    this->program = program;
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].Load(program);
	}
}

void Temple::draw()
{
	string oFName;
	vec4 t;
	int i = 0;
	int li = 0;

	//Draws models
	for (int x = 0; x < objects.size(); x++)
	{
		//take the file name and map it to the correct location
		//for each location, translate and draw the corresponding
		//object
		for (auto l : locations[objNames[x]])
		{

				objects[x].translateObj(vec3(l.x, l.y, l.z) + translate);
                if(objNames[x] == "Light" && li < ptempleLights.size())
                {
                    ptempleLights[li].translateLight(translate);
                    ptempleLights[li].transferSettings(program);
                    li++;
                }
				if (checkFrustrum(objects[x].provideBoundingBox()))
					objects[x].Draw();

		}

	}

}

/*Reads the file containg the object names, locations etc
Stores the locations, rotations and scales in maps : object names to vectors
The object names are stored in a vector
*/
void Temple::readPlacementFile()
{
	ifstream infile;
	vec4 temp;
	vec3 temp2, temp3;
	string Oname = "", nOname = "";
	char c;
	bool sameName;

	vector<vec4> l;
	vector<vec3> r;
	vector<vec3> s;


	infile.open(datPath);
	infile >> Oname;
	nOname = Oname;
	while (infile)
	{
		sameName = false;
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

			if (!sameName)
			{
				sameName = true;
				objNames.push_back(Oname);
			}
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

void Temple::translateTemple(vec3 t)
{
	translate = t;
}

Temple::~Temple()
{
}
