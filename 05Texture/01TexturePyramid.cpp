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

// render a textured pyramid

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float pyrLocX, pyrLocY, pyrLocZ;
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

void setupVertices()
{
    // the pyramid is constructed by 6 triangles
    float pyramidPositions[54] = {
        -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  // front
         1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  // right
         1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  // back
        -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  // left
        -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  // bottom: left-front half
         1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f   // bottom: right-back half
    };
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);

    // texture coordinates for texture of pyramid, 18 points x 2d texture coordinats per point.
    float pyramidTextureCoords[36] = {
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // front
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // right
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // back
        0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f, // left
        0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, // bottom: left-front half
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f  // bottom: right-back half
    };
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // for texture
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidTextureCoords), pyramidTextureCoords, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("01Vertex.glsl", "01Fragment.glsl");
    cameraX = 0.0f;
    cameraY = 8.0f;
    cameraZ = 8.0f;
    
    // pyramid
    pyrLocX = 0.0f;
    pyrLocY = 0.0f;
    pyrLocZ = 0.0f;

    setupVertices();
    
    // set up textures
    brickTexture = Utils::loadTexture("01BrickTExture.jpg");

    // recalculate the perspective projection matrix when window size changes
    auto calculatePerspectiveMatrix = [](GLFWwindow* w, int newWidth, int newHeight) -> void
    {
        aspect = float(newWidth) / float(newHeight);
        glViewport(0, 0, newWidth, newHeight); // set the screen area associated with the frame buffer (aka the view port).
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    };
    glfwSetWindowSizeCallback(window, calculatePerspectiveMatrix);

    // build the perspective proejction matrix
    glfwGetFramebufferSize(window, &width, &height); // screen width and height
    calculatePerspectiveMatrix(window, width, height);
    
    // view matrix
    // vMat = glm::rotate(glm::mat4(1.0f), glm::pi<float>()/4.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    // vMat *= glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    vMat = glm::lookAt(glm::vec3(0.0f, 4.0f, 4.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, -1));
}

float angle = 0.0;

std::stack<glm::mat4> mvStack;

void display(GLFWwindow* window, double currentTime)
{
    // clear background to black during every rendering
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // load program to GPU, not run program
    glUseProgram(renderingProgram);

    // get uniform variable of model-view matrix and persepctive projection matrix
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    // view matrix to be the bottom
    mvStack.push(vMat);

    // perspective projections are all the same
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // backface culling (for efficiency)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // default to GL_BACK, no need to write this line.

    // draw the pyramid as the solar
    mvStack.push(mvStack.top());
    mvStack.top() *=  glm::translate(glm::mat4(1.0f), glm::vec3(pyrLocX, pyrLocY, pyrLocZ));
    mvStack.push(mvStack.top());
    mvStack.top() *= glm::rotate(glm::mat4(1.0f), float(currentTime), glm::vec3(1.0f, 0.0f, 0.0f)); // self-rotation of solar

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvStack.top()));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // pyramid
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    // texture buffer: in index 1
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // active and bind texture for pyramid
    glActiveTexture(GL_TEXTURE0); // make texture unit 0 (in hardware) active
    glBindTexture(GL_TEXTURE_2D, brickTexture); // bind brickTexture to the active texture unit.

    glFrontFace(GL_CCW); // specify counter clock-wise as front face, the default case for glFrontFace & the usual case for models.
    glDrawArrays(GL_TRIANGLES, 0, 18);
    mvStack.pop();

    // clear the matrix stack
    while (!mvStack.empty())
    {
        mvStack.pop();
    }
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
    window = glfwCreateWindow(1920, 1080, "01TexturePyramid", NULL, NULL);
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
