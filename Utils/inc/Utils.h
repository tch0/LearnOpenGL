#pragma once
#include <glad/gl.h>
#include <string>
#include <source_location>

namespace Utils
{

// error handling
// for GLSL compile error
void printShaderLog(GLuint shader, const std::source_location& loc = std::source_location::current());
// for GLSL link error
void printProgramLog(GLuint program, const std::source_location& loc = std::source_location::current());
// check for general OPenGL error
bool checkOpenGLError(const std::source_location& loc = std::source_location::current());

// read shader source from file
std::string readShaderSource(const char* filePath, const std::source_location& loc = std::source_location::current());

// create a program from vertex shader and fragment shader
GLuint createShaderProgram(const char* vertexShader, const char* fragmentShader, const std::source_location& loc = std::source_location::current());

// create shader program from vertex shader and fragment shader sources
GLuint createShaderProgramFromSource(const char* vertexShader, const char* fragmentShader, const std::source_location& loc = std::source_location::current());
GLuint createShaderProgramFromSource(const char* vertexShader, const char* fragmentShader, const char* geometryShader, const std::source_location& loc = std::source_location::current());

// load texture to OpenGL texture object
GLuint loadTexture(const char* textureImagePath, const std::source_location& loc = std::source_location::current());

// load cube map texture to OpenGL texture object
GLuint loadCubeMap(const char* rightImage, const char* leftImage,
                   const char* topImage, const char* bottomImage,
                   const char* frontImage, const char* backImage,
                   const std::source_location& loc = std::source_location::current());

} // namespace Utils