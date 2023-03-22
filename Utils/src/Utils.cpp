#include <Utils.h>
#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <soil2/SOIL2.h>
#include <Logger.h>

namespace Utils
{

// error handling
// for GLSL compile error
void printShaderLog(GLuint shader, const std::source_location& loc)
{
    GLint len = 0;
    GLint chWritten = 0;
    char* log = nullptr;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = new char[len];
        glGetShaderInfoLog(shader, len, &chWritten, log);
        Logger::globalLogger().info(std::format("Shader Info Log: \n{}", log), loc);
        delete[] log;
    }
}
// for GLSL link error
void printProgramLog(GLuint program, const std::source_location& loc)
{
    GLint len = 0;
    GLint chWritten = 0;
    char* log = nullptr;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 0)
    {
        log = new char[len];
        glGetProgramInfoLog(program, len, &chWritten, log);
        Logger::globalLogger().info(std::format("Shader Info Log: \n{}", log), loc);
        delete[] log;
    }
}

static std::string glErrorToString(GLenum glError)
{
    switch(glError)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM"s;
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE"s;
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION"s;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION"s;
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY"s;
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW"s;
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW"s;
    default:
        return std::to_string(glError);
    }
}

// check for general OPenGL error
bool checkOpenGLError(const std::source_location& loc)
{
    bool foundError = false;
    GLenum glErr = glGetError();
    while (glErr != GL_NO_ERROR)
    {
        Logger::globalLogger().warning(std::format("OpenGL error: {}", glErrorToString(glErr)), loc);
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

// read shader source from file
std::string readShaderSource(const char* filePath, const std::source_location& loc)
{
    std::string content;
    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        Logger::globalLogger().warning(std::format("Shader file {} does not exist!", filePath), loc);
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
GLuint createShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile, const std::source_location& loc)
{
    std::string vertexShaderStr = readShaderSource(vertexShaderFile, loc);
    std::string fragShaderStr = readShaderSource(fragmentShaderFile, loc);
    return createShaderProgramFromSource(vertexShaderStr.c_str(), fragShaderStr.c_str(), loc);
}

// create a shader program from shader sources
GLuint createShaderProgramFromSource(const char* vertexShader, const char* fragmentShader, const std::source_location& loc)
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
        Logger::globalLogger().warning("Vertex shader compilation failed!", loc);
        printShaderLog(vShader, loc);
    }
    glCompileShader(fShader);
    checkOpenGLError();
    GLint fragCompiled = GL_FALSE;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != GL_TRUE)
    {
        Logger::globalLogger().warning("Fragment shader compilation failed!", loc);
        printShaderLog(fShader, loc);
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
        Logger::globalLogger().warning("Shader program linking failed!", loc);
        printProgramLog(vfProgram, loc);
    }
    return vfProgram;
}

GLuint createShaderProgramFromSource(const char* vertexShader, const char* fragmentShader, const char* geometryShader, const std::source_location& loc)
{
    // create empty shader object
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLuint gShader = glCreateShader(GL_GEOMETRY_SHADER);
    
    // load glsl source to shader object
    glShaderSource(vShader, 1, &vertexShader, NULL);
    glShaderSource(fShader, 1, &fragmentShader, NULL);
    glShaderSource(gShader, 1, &geometryShader, NULL);

    // compiler shader obejct
    glCompileShader(vShader);
    checkOpenGLError();
    GLint vertCompiled = GL_FALSE;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    if (vertCompiled != GL_TRUE)
    {
        Logger::globalLogger().warning("Vertex shader compilation failed!", loc);
        printShaderLog(vShader, loc);
    }
    glCompileShader(fShader);
    checkOpenGLError();
    GLint fragCompiled = GL_FALSE;
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != GL_TRUE)
    {
        Logger::globalLogger().warning("Fragment shader compilation failed!", loc);
        printShaderLog(fShader, loc);
    }
    glCompileShader(gShader);
    checkOpenGLError();
    GLint geometryCompiled = GL_FALSE;
    glGetShaderiv(gShader, GL_COMPILE_STATUS, &geometryCompiled);
    if (geometryCompiled != GL_TRUE)
    {
        Logger::globalLogger().warning("Geometry shader compilation failed!", loc);
        printShaderLog(gShader, loc);
    }
    
    // create program, attach shaders to program (in GPU)
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glAttachShader(vfProgram, gShader);
    glLinkProgram(vfProgram);
    checkOpenGLError();
    GLint linkStatus = GL_FALSE;
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        Logger::globalLogger().warning("Shader program linking failed!", loc);
        printProgramLog(vfProgram, loc);
    }
    return vfProgram;
}

// load texture to OpenGL texture object
GLuint loadTexture(const char* textureImagePath, const std::source_location& loc)
{
    GLuint textureId;
    textureId = SOIL_load_OGL_texture(textureImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureId == 0)
    {
        Logger::globalLogger().warning(std::format("Could not find texture file {}!", textureImagePath), loc);
    }
    return textureId;
}

} // namespace Utils