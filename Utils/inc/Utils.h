#pragma once
#include <glad/gl.h>
#include <string>

namespace Utils
{

// error handling
// for GLSL compile error
void printShaderLog(GLuint shader);
// for GLSL link error
void printProgramLog(GLuint program);
// check for general OPenGL error
bool checkOpenGLError();

// read shader source from file
std::string readShaderSource(const char* filePath);

// create a program from vertex shader and fragment shader
GLuint createShaderProgram(const char* vertexShader, const char* fragmentShader);

// create shader program from vertex shader and fragment shader sources
GLuint createShaderProgramFromSource(const char* vertexShader, const char* fragmentShader);

// load texture to OpenGL texture object
GLuint loadTexture(const char* textureImagePath);

} // namespace Utils