#include "Loader.h"
#include <iostream>

Loader::Loader(){
}

SimpleModel Loader::loadToVao(vector<float> vertices){
    GLuint vaoID = createVao();
    storeDataInVao(0, 3, vertices);
    glBindVertexArray(0);

    SimpleModel model(vaoID, vertices.size()/3);
    return model;
}

TexturedModel Loader::loadToVao(vector<float> vertices, vector<float> textures, GLuint texID){
    GLuint vaoID = createVao();
    storeDataInVao(0, 3, vertices);
    storeDataInVao(2, 2, textures);
    glBindVertexArray(0);

    TexturedModel model(vaoID, texID, vertices.size()/3);
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

GLuint Loader::loadTexture(string filename, int unit){
    string full_path = "textures\\" + filename + ".jpg";

    cout << full_path << endl;
    GLint width, height;
    unsigned char* image = SOIL_load_image(full_path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    cout <<     SOIL_last_result() << endl;
    GLuint tex;

    if(image){
        glActiveTexture(unit);
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        //Apply settings

        //Load the texture image to the active texture unit
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
        textures.push_back(tex);
    }
    else{
       tex = -1;
    }

    return tex;
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

    for(auto tex: textures)
    {
        glDeleteTextures(1, &tex);
    }

}

Loader::~Loader(){
}
