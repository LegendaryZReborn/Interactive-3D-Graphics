#include "Loader.h"

Loader::Loader(){
}

SimpleModel Loader::loadToVao(vector<float> vertices){
    GLuint vaoID = createVao();
    storeDataInVao(0, 3, vertices);
    glBindVertexArray(0);

    SimpleModel model(vaoID, vertices.size()/3);
    return model;
}

GLuint Loader::createVao(){
    GLuint vaoID;
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    vaos.push_back(vaoID);
    return vaoID;
}

void Loader::storeDataInVao(GLuint attributeNum, int size, vector<float> data){
    GLuint vboID;
    glGenBuffers(1, &vboID);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

    int dataSize = data.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, dataSize, &data[0], GL_STATIC_DRAW);
    glVertexAttribPointer(attributeNum, size, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(attributeNum);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    vbos.push_back(vboID);
}

void Loader::cleanUp(){
    for(auto vao: vaos)
    {
        glDeleteVertexArrays(1, &vao);
    }

    for(auto vbo: vbos)
    {
        glDeleteBuffers(1, &vbo);
    }
}

Loader::~Loader(){
}
