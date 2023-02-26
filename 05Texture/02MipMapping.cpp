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

// mipmapping test

#define numVAOs 1
#define numVBOs 2

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// spaces that display use, avoid allocate in display every time
GLuint mvLoc, projLoc, mipmapLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat; // projection, view, model, model-view
// textures
GLuint bwTexture, mipmappingBwTexture;

void setupVertices()
{
    // the pyramid is constructed by 6 triangles
    float rectPositions[18] = {
        -20.0f, 0.0f, -20.0f, -20.0f, 0.0f,  20.0f,  20.0f, 0.0f,  20.0f, // left-front
         20.0f, 0.0f,  20.0f,  20.0f, 0.0f, -20.0f, -20.0f, 0.0f, -20.0f  // right-back
    };
    float textureCoords[12] = {
         0.0f, 20.0f,  0.0f,  0.0f, 20.0f,  0.0f,
        20.0f,  0.0f, 20.0f, 20.0f,  0.0f, 20.0f
    };
    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectPositions), rectPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // for texture
    glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
}

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("02Vertex.glsl", "02Fragment.glsl");
    setupVertices();
    
    // set up textures
    bwTexture = Utils::loadTexture("02BlackWhiteGrid.jpg");
    mipmappingBwTexture = Utils::loadTexture("02BlackWhiteGrid.jpg");

    // recalculate the perspective projection matrix when window size changes
    auto calculatePerspectiveMatrix = [](GLFWwindow* w, int newWidth, int newHeight) -> void
    {
        width = newWidth;
        height = newHeight;
        aspect = float(newWidth / 2) / float(newHeight);
        glViewport(0, 0, newWidth/2, newHeight); // set the screen area associated with the frame buffer (aka the view port).
        pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
    };
    glfwSetWindowSizeCallback(window, calculatePerspectiveMatrix);

    // build the perspective proejction matrix
    glfwGetFramebufferSize(window, &width, &height); // screen width and height
    calculatePerspectiveMatrix(window, width, height);
    
    // view matrix
    vMat = glm::lookAt(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0, 10, -1));
}

void displayPlane(bool mipmapping, float currentTime)
{
    // non-mipmapping on left, mipmapping on right
    if (!mipmapping)
    {
        glViewport(0, 0, width/2, height);
    }
    else
    {
        glViewport(width/2, 0, width/2, height);
    }

    // load program to GPU, not run program
    glUseProgram(renderingProgram);

    // get uniform variable of model-view matrix and persepctive projection matrix
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");
    mipmapLoc = glGetUniformLocation(renderingProgram, "doMipmapping");

    // perspective projections are all the same
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // backface culling (for efficiency)
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // default to GL_BACK, no need to write this line.
    glFrontFace(GL_CCW);

    // model-view matrix
    mMat = glm::rotate(glm::mat4(1.0f), currentTime, glm::vec3(0.0f, 1.0f, 0.0f)); // rotate around y-axis
    mvMat = vMat * mMat;

    // rectanble
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // texture
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // texture unit
    if (mipmapping)
    {
        // use mipmapping
        glUniform1i(mipmapLoc, 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, mipmappingBwTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D); // generate mipmapping
        if (GLAD_GL_EXT_texture_filter_anisotropic) // check if anisotropic filtering extension is supported ?
        {
            GLfloat anisoSetting = 0.0f;
            glGetFloatv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
            glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting); // enable anisotropic filtering
        }
    }
    else
    {
        // do not use mipmapping
        glUniform1i(mipmapLoc, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, bwTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void display(GLFWwindow* window, double currentTime)
{
    // clear background to black during every rendering
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    displayPlane(true, float(currentTime));
    displayPlane(false, float(currentTime));
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
    window = glfwCreateWindow(1920, 1080, "02MipMapping", NULL, NULL);
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
