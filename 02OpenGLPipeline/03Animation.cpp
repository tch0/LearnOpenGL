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

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("03Vertex.glsl", "03Fragment.glsl");
    // create vertex array objects
    glGenVertexArrays(numVAOs, vao);
    glBindVertexArray(vao[0]);
}

float x = 0.0f;
float inc = 0.01f;

void display(GLFWwindow* window, double currentTime)
{
    // clear background to black during every rendering
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // load program to GPU, not run program
    glUseProgram(renderingProgram);

    // move the triangle back and forth
    x += inc;
    if (x > 1.0f)
    {
        inc = -0.01f;
    }
    else if (x < -1.0f)
    {
        inc = 0.01f;
    }
    GLuint offsetLoc = glGetUniformLocation(renderingProgram, "offset"); // get offset pointer
    glProgramUniform1f(renderingProgram, offsetLoc, x); // pass x to offset, it can be accessed in vertex shader
    // start rendering pipeline
    glDrawArrays(GL_TRIANGLES, 0, 3); // 3 points for triangle

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
