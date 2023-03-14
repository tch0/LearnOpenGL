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
#include <stack>
#include <Utils.h>
#include <Sphere.h>

// generate a sphere through a program

#define numVAOs 1
#define numVBOs 3

// the sphere model
Utils::Sphere mySphere(48);
GLsizei verticesCount;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// spaces that display use, avoid allocate in display every time
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat; // projection, view, model, model-view

void setupVertices()
{
    std::vector<float> vertices = mySphere.getVerticesArray();
    std::vector<float> texCoords = mySphere.getTexCoordsArray();
    std::vector<float> normals = mySphere.getNormalsArray();

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    verticesCount = vertices.size() / 3;
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    // texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
    // normal coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("01Vertex.glsl", "01Fragment.glsl");
    setupVertices();
    
    // set up textures

    // recalculate the perspective projection matrix when window size changes
    auto calculatePerspectiveMatrix = [](GLFWwindow* w, int newWidth, int newHeight) -> void
    {
        width = newWidth;
        height = newHeight;
        aspect = float(newWidth) / float(newHeight);
        glViewport(0, 0, newWidth, newHeight); // set the screen area associated with the frame buffer (aka the view port).
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    };
    glfwSetWindowSizeCallback(window, calculatePerspectiveMatrix);

    // build the perspective proejction matrix
    glfwGetFramebufferSize(window, &width, &height); // screen width and height
    calculatePerspectiveMatrix(window, width, height);
    
    // view matrix
    vMat = glm::lookAt(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, -1));
}

void displaySphere(float currentTime)
{
    // load program to GPU, not run program
    glUseProgram(renderingProgram);

    // get uniform variable of model-view matrix and persepctive projection matrix
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    // perspective projections are all the same
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // backface culling (for efficiency)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // default to GL_BACK, no need to write this line.
    glFrontFace(GL_CCW);

    // model-view matrix
    // mMat = glm::mat4(1.0f);
    mMat = glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y-axis over time
    mvMat = vMat * mMat;

    // sphere vertices
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // draw
    glDrawArrays(GL_LINE_STRIP, 0, verticesCount); // GL_TRIANGLES to render the sphere surface
}

void display(GLFWwindow* window, double currentTime)
{
    // clear background to black during every rendering
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    displaySphere(float(currentTime));
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
    window = glfwCreateWindow(1920, 1080, "01GeneratedSphere", NULL, NULL);
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
