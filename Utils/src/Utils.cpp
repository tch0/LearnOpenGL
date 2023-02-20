#include <Utils.h>
#include <iostream>
#include <fstream>
#include <string>

namespace Utils
{

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
        delete[] log;
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
        delete[] log;
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

// read shader source from file
std::string readShaderSource(const char* filePath)
{
    std::string content;
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        std::cout << "Shader file " << filePath << " does not exist!" << std::endl;
        return content;
    }
    std::string line;
    while (!fin.eof())
    {
        std::getline(fin, line);
        content += line + "\n";
    }
    fin.close();
    return content;
}

// create a program from shaders
GLuint createShaderProgram(const char* vertexShader, const char* fragmentShader)
{
    std::string vertexShaderStr = readShaderSource(vertexShader);
    std::string fragShaderStr = readShaderSource(fragmentShader);
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

} // namespace Utils