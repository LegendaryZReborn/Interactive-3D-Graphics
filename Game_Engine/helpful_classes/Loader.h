#ifndef LOADER_H
#define LOADER_H
#include "SimpleModel.h"
#include <vector>

using namespace std;

class Loader
{
    public:
        Loader();
        ~Loader();

        SimpleModel loadToVao(vector<float> vertices);
        void cleanUp();

    protected:

    private:
       GLuint createVao();
       void storeDataInVao(GLuint attributeNum, int size, vector<float> data);

       vector<GLuint> vaos;
       vector<GLuint> vbos;

};

#endif // LOADER_H
