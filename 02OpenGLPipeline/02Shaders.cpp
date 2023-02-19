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

// error handling
// for GLSL compile error
void printShaderLog(GLuint shader)
{
    GLint len = 0;
    GLint chWritten = 0;
    char* log = nullptr;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = new char[len];
        glGetShaderInfoLog(shader, len, &chWritten, log);
        std::cout << "Shader Info Log: " << log << std::endl;
        delete log;
    }
}
// for GLSL link error
void printProgramLog(GLuint program)
{
    GLint len = 0;
    GLint chWritten = 0;
    char* log = nullptr;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = new char[len];
        glGetProgramInfoLog(program, len, &chWritten, log);
        std::cout << "Shader Info Log: " << log << std::endl;
        delete log;
    }
}
// check for general OPenGL error
bool checkOpenGLError()
{
    bool foundError = false;
    GLenum glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        std::cout << "glError: " << glErr << std::endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

// definition of points
#define numVAOs 1
GLuint renderingProgram;
GLuint vao[numVAOs];

// read shader source from file
std::string readShaderSource(const char* filePath)
{
    std::string content;
    std::ifstream fin(filePath);
    std::string line;
    while (!fin.eof())
    {
        std::getline(fin, line);
        content += line + "\n";
    }
    fin.close();
    return content;
}

// shader
// paint a white pixel point
GLuint createShaderProgram()
{
    std::string vertexShaderStr = readShaderSource("02Vertex.glsl");
    std::string fragShaderStr = readShaderSource("02Fragment.glsl");
    // vertex shader will called by every vertex
    const char* vshaderSource = vertexShaderStr.c_str();
    // fragment shader will give the pixels color
    const char* fshaderSource = fragShaderStr.c_str();

    // create empty shader object
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // load glsl source to shader object
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);

    // compiler shader obejct
    glCompileShader(vShader);
    checkOpenGLError();
    GLint vertCompiled = GL_FALSE;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != GL_TRUE)
    {
        std::cout << "Vertex shader compilation failed!" << std::endl;
        printShaderLog(vShader);
    }
    glCompileShader(fShader);
    checkOpenGLError();
    GLint fragCompiled = GL_FALSE;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != GL_TRUE)
    {
        std::cout << "Fragment shader compilation failed!" << std::endl;
        printShaderLog(fShader);
    }
    
    // create program, attach shaders to program (in GPU)
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        std::cout << "Linking failed!" << std::endl;
        printProgramLog(vfProgram);
    }
    return vfProgram;
}

/*
in this case:
       vertex shader              rasterization shader  fragment shader
point ---------------------> point -------------> pixel --------------> pixels in screen
       process every point                               add color
*/

void init(GLFWwindow* window)
{
    renderingProgram = createShaderProgram();
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
