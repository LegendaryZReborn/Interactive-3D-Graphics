#ifndef LOADER_H
#define LOADER_H
#include "SimpleModel.h"
#include "TexturedModel.h"
#include "Soil/SOIL.h"
#include <vector>
#include <string>

using namespace std;

class Loader{
    public:
        Loader();
        ~Loader();

        SimpleModel loadToVao(vector<float> vertices);
        TexturedModel loadToVao(vector<float> vertices, vector<float> textures, GLuint texID);
        GLuint loadTexture(string filename, int unit);
        void cleanUp();

    protected:

    private:
       GLuint createVao();
       void storeDataInVao(GLuint attributeNum, int size, vector<float> data);

       vector<GLuint> vaos;
       vector<GLuint> vbos;
       vector<GLuint> textures;

};

#endif // LOADER_H
