#include "TexturedModel.h"

TexturedModel::TexturedModel(): SimpleModel(){
    //ctor
}

TexturedModel::TexturedModel(GLuint vaoID, GLuint texID, int numVert):SimpleModel(vaoID, numVert){
    tex = texID;
}

GLuint TexturedModel::getTex(){
    return tex;
}

TexturedModel::~TexturedModel(){
    //dtor
}
