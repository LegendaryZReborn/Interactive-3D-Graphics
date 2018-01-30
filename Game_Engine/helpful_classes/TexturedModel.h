#ifndef TEXTUREDMODEL_H
#define TEXTUREDMODEL_H

#include <SimpleModel.h>


class TexturedModel : public SimpleModel
{
    public:
        TexturedModel();
        TexturedModel(GLuint vaoID, GLuint texID, int numVert);
        GLuint getTex();
       ~TexturedModel();

    private:
        GLuint tex;
};

#endif // TEXTUREDMODEL_H
