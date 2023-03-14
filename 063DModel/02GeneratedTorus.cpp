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
#include <Torus.h>

// generate a torus through a program

#define numVAOs 1
#define numVBOs 4

// the torus model
Utils::Torus myTorus(1.0f, 3.0f, 48);
GLsizei verticesCount;

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// spaces that display use, avoid allocate in display every time
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat; // projection, view, model, model-view
// texture
GLuint brickTexture;

std::vector<int> indices;

void setupVertices()
{
    // std::vector<int> 
    indices = myTorus.getIndices();
    std::vector<glm::vec3> vertices = myTorus.getVertices();
    std::vector<glm::vec2> texCoords = myTorus.getTexCoords();
    std::vector<glm::vec3> normals = myTorus.getNormals();

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    verticesCount = indices.size();
    // Note: we assum and depend that glm::vec3 and glm::vec2 are tightly arranged !!
    // if vertices data is got by getVerticesArray, use glDrawArrays !!
    assert(sizeof(glm::vec3) == 3 * sizeof(float));
    assert(sizeof(glm::vec2) == 2 * sizeof(float));
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    // texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
    // normal coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    // indices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("02Vertex.glsl", "02Fragment.glsl");
    setupVertices();
    
    // set up textures
    brickTexture = Utils::loadTexture("02BrickTexture.jpg");

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
    vMat = glm::lookAt(glm::vec3(0.0f, 6.0f, 6.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, -1));
}

void displayTorus(float currentTime)
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
    mMat = glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.0f, 0.0f, 1.0f)); // rotate around z-axis over time
    mvMat = vMat * mMat;

    // torus vertices
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

    // indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[3]);

    // texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, brickTexture);

    // draw: use indexed points to save space.
    glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, 0); // use indices
}

void display(GLFWwindow* window, double currentTime)
{
    // clear background to black during every rendering
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    displayTorus(float(currentTime));
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
    window = glfwCreateWindow(1920, 1080, "02GeneratedTorus", NULL, NULL);
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
