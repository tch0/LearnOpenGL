#include <Shader.h>
#include <Utils.h>
#include <Renderer.h>

const char* vertexShader = R"glsl(
#version 430
uniform mat4 mvp_matrix;
void main() {}
)glsl";

const char* tessCtrlShader = R"glsl(
#version 430
uniform mat4 mvp_matrix;
layout (vertices = 1) out;
void main()
{
    gl_TessLevelOuter[0] = 6;
    gl_TessLevelOuter[1] = 6;
    gl_TessLevelOuter[2] = 6;
    gl_TessLevelOuter[3] = 6;
    gl_TessLevelInner[0] = 12;
    gl_TessLevelInner[1] = 12;
}
)glsl";

const char* tessEvalShader = R"glsl(
#version 430
uniform mat4 mvp_matrix;
layout (quads, equal_spacing, ccw) in;
void main()
{
    // gl_TesCoord visit position of vertices (in range [0.0, 1.0]) that are generated by tessellation control shader
    float u = gl_TessCoord.x; 
    float v = gl_TessCoord.y;
    gl_Position = mvp_matrix * vec4(u, 0, v, 1);
}
)glsl";

const char* fragmentShader = R"glsl(
#version 430
out vec4 color;
uniform mat4 mvp_matrix;
void main()
{
    color = vec4(1.0, 1.0, 0.0, 1.0); // yellow
}
)glsl";


int main(int argc, char const *argv[])
{
    Utils::Renderer renderer("01TessExample");
    Utils::Shader tessShader(vertexShader, tessCtrlShader, tessEvalShader, fragmentShader);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);

    auto display = [&](GLFWwindow* pWindow, float currentTime) -> void {
        glm::mat4 mMat(1.0f);
        glm::mat4 vMat = glm::lookAt(Utils::Renderer::getEyeLocation(pWindow), Utils::Renderer::getObjectLocation(pWindow), Utils::Renderer::getUpVector(pWindow));
        glm::mat4 pMat = Utils::Renderer::getProjMatrix(pWindow);
        
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        
        renderer.drawAxises();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);

        tessShader.use();
        tessShader.setMat4("mvp_matrix", pMat * vMat * mMat);
        glPatchParameteri(GL_PATCH_VERTICES, 1);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBindVertexArray(vao);
        glDrawArrays(GL_PATCHES, 0, 1); // all shader program with a tessellation ctrl and eval shader should use GL_PATCHES primitive type
        Utils::checkOpenGLError();
    };
    renderer.setDisplayCallback(display);
    renderer.run();
    return 0;
}
