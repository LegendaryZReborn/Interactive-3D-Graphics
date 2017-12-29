#ifndef SHADER_H
#define SHADER_H
#include<GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class Shader{
    public:
        Shader();
        Shader(string vertexShader, string fragmentShader);
        ~Shader();

        void startShader();
        void stopShader();
        void cleanUpShader();

    protected:
        GLuint programID;
        GLuint vertexShaderID;
        GLuint fragmentShaderID;

        GLuint loadShader(string sourceFileName, GLenum type);

};

#endif // SHADER_H
