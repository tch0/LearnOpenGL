#include <Utils.h>
#include <iostream>
#include <fstream>
#include <string>
#include <soil2/SOIL2.h>
#include <Logger.h>

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
        Logger::globalLogger().info("Shader Info Log: "s + log);
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
        Logger::globalLogger().info("Shader Info Log: \n"s + log);
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
        Logger::globalLogger().warning("glError: "s + std::to_string(glErr));
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
        Logger::globalLogger().warning("Shader file "s + filePath + " does not exist!"s);
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

// create a program from shaders files
GLuint createShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile)
{
    std::string vertexShaderStr = readShaderSource(vertexShaderFile);
    std::string fragShaderStr = readShaderSource(fragmentShaderFile);
    return createShaderProgramFromSource(vertexShaderStr.c_str(), fragShaderStr.c_str());
}

// create a shader program from shader sources
GLuint createShaderProgramFromSource(const char* vertexShader, const char* fragmentShader)
{
    // create empty shader object
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    // load glsl source to shader object
    glShaderSource(vShader, 1, &vertexShader, NULL);
    glShaderSource(fShader, 1, &fragmentShader, NULL);

    // compiler shader obejct
    glCompileShader(vShader);
    checkOpenGLError();
    GLint vertCompiled = GL_FALSE;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != GL_TRUE)
    {
        Logger::globalLogger().warning("Vertex shader compilation failed!");
        printShaderLog(vShader);
    }
    glCompileShader(fShader);
    checkOpenGLError();
    GLint fragCompiled = GL_FALSE;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != GL_TRUE)
    {
        Logger::globalLogger().warning("Fragment shader compilation failed!");
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
        Logger::globalLogger().warning("Linking failed!");
        printProgramLog(vfProgram);
    }
    return vfProgram;
}

// load texture to OpenGL texture object
GLuint loadTexture(const char* textureImagePath)
{
    GLuint textureId;
    textureId = SOIL_load_OGL_texture(textureImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureId == 0)
    {
        Logger::globalLogger().warning("Utils::loadTexture: Could not find texture file!");
    }
    return textureId;
}

} // namespace Utils