#include "Temple.h"

Temple::Temple()
{
	string n;

	datPath = "Files\\placement.dat";
	fileNamePath = "C:\\Users\\Cavaughn\\Documents\\Visual Studio 2015";
	fileNamePath += "\\Projects\\Graphics\\Island Project\\GraphicsProject3_ReadModels\\Files\\";
	fileNamePath += "TempleFileNames.txt";

	readPlacementFile();

	bool firstPillar = true;
	size_t pos;
	for each(string m in objNames)
	{

		pos = m.find("Full_Pillar");

		if (pos == string::npos)
		{
			n = "Temple\\" + m + ".obj";

			objects.push_back(Object(n));

		}
		else
		{
			if (firstPillar)
			{
				firstPillar = false;
				n = "Temple\\" + m + ".obj";
				objects.push_back(Object(n));
			}
		}
		
	}


}

void Temple::load()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].Load();
	}
}

void Temple::draw()
{
	string oFName;
	size_t pos;
	//Draws models
	for (int x = 0; x < objects.size(); x++)
	{
		//take the file name and map it to the correct location
		//for each location, translate and draw the corresponding 
		//object
		for (auto l : locations[objNames[x]])
		{
		
				//object[x].scaleObj(vec3())
				objects[x].translateObj(vec3(l.x, l.y, l.z));
				objects[x].Draw();
			
		}

		//locations[];
		//objects[x].translateObj();
		//objects[].Draw();

	}
	
}

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


Temple::~Temple()
{
}
