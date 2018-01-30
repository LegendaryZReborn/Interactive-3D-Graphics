#include<iostream>
#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include "Loader.h"
#include "SimpleModel.h"
#include "TexturedModel.h"
#include "Shader.h"

using namespace std;

//Prints a description of a glfw related error
void glfwError(int error, const char* description);
//Sets the width and height of the glViewPort if window is reszied
void reshape(GLFWwindow* window, int width, int height);
//Handles key input events
void handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
//Initializes all the settings, etc for the rest of the program
void initEngine();

int main(void){

    GLFWwindow* window;

    glfwSetErrorCallback(glfwError);
    /*Initialize the library*/
    if(!glfwInit()){
        /*Initialization failed*/
        return -1;
    }

    window = glfwCreateWindow(1280, 720, "I'm Back Bitches", NULL, NULL);

    if(!window){
        /*Window creation failed*/
        glfwTerminate();
        return -1;
    }
    /*Make the window's context current*/
    glfwMakeContextCurrent(window);

    glewInit();
     /*Sets the call back function for when window is resized*/
    glfwSetFramebufferSizeCallback(window, reshape);
    /*Sets the callback function for key input events*/
    glfwSetKeyCallback(window, handleKeyInput);

    vector<float> rectVerts = {
        //Left-bottom triangle
        -0.5, 0.5, 0,
        -0.5, -0.5, 0,
        0.5, -0.5, 0,
        //Right-top triangle
        0.5, -0.5, 0,
        0.5, 0.5, 0,
        -0.5, 0.5, 0
    };

    vector<float> textureUVs = {
        //Left-bottom triangle
        0.0, 0.0,
        0.0, 1.0,
        1.0, 1.0,
        //Right-top triangle
        1.0, 1.0,
        1.0, 0.0,
        0.0, 0.0
    };

    Loader loader;
    GLuint sqTex = loader.loadTexture("goku", 0);

    TexturedModel rect = loader.loadToVao(rectVerts, textureUVs, sqTex);
    Shader shader("shaders/v_shader.glsl", "shaders/f_shader.glsl");

    /*Loop while the window hasn't been close*/
    while(!glfwWindowShouldClose(window)){
        /*Clear the buffer*/
        glClearColor(1.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.startShader();
         glUniform1f(1, (GLfloat)glfwGetTime());
        /*Render*/
        glBindVertexArray(rect.GetvaoID());
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(2);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, rect.getTex());
        glDrawArrays(GL_TRIANGLES, 0, rect.GetnumVertices());
        glDisableVertexAttribArray(0);
        glEnableVertexAttribArray(2);
        glBindVertexArray(0);
        shader.stopShader();

        /*Swap front and back buffers after rendering to back for seamless image*/
        glfwSwapBuffers(window);

        /*Poll for and process events */
        glfwPollEvents();
    }

    loader.cleanUp();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void glfwError(int error, const char* description){
    fprintf(stderr, "Error: %s\n", description);
}

void reshape(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void handleKeyInput(GLFWwindow* window, int key, int scancode, int action, int mods){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void initEngine(){

}

