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

} // namespace Utils