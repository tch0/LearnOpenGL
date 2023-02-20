#include <glad/gl.h> // glad2!
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <soil2/SOIL2.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cassert>
#include <string>
#include <fstream>
#include <Utils.h>

// definition of points
#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];

/*
in this case:
       vertex shader              rasterization shader  fragment shader
point ---------------------> point -------------> pixel --------------> pixels in screen
       process every point                               add color
*/

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("02Vertex.glsl", "02Fragment.glsl");
    // create vertex array objects
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime)
{
    // load program to GPU, not run program
    glUseProgram(renderingProgram);
    // start rendering pipeline
    glDrawArrays(GL_POINTS, 0, 1);
    glPointSize(30.0f);
}

int main(int argc, char const *argv[])
{
    GLFWwindow* window;
    // init glfw
    if (!glfwInit())
    {
        return -1;
    }
    // OpenGL compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // create window
    window = glfwCreateWindow(1920, 1080, "hello, world!", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // init glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);
    init(window);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
