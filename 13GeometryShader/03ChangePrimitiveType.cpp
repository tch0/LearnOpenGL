#include <Shader.h>
#include <Utils.h>
#include <Renderer.h>
#include <Torus.h>

const char* vertexShader = R"glsl(
#version 430
layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 vertexNormal;
uniform mat4 mvMatrix;
uniform mat4 normMatrix;
uniform mat4 projMatrix;

out vec3 varyingNormal;
void main()
{
    gl_Position = mvMatrix * vec4(vertexPos, 1.0f);
    varyingNormal = (normMatrix * vec4(vertexNormal, 1.0f)).xyz;
}
)glsl";

const char* geometryShader = R"glsl(
#version 430
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;
uniform mat4 mvMatrix;
uniform mat4 normMatrix;
uniform mat4 projMatrix;

in vec3 varyingNormal[];
void main()
{
    // change primitive type from triangle to line
    vec3 op0 = gl_in[0].gl_Position.xyz;
    vec3 op1 = gl_in[1].gl_Position.xyz;
    vec3 op2 = gl_in[2].gl_Position.xyz;
    vec3 ep0 = op0 + varyingNormal[0] * 0.5;
    vec3 ep1 = op1 + varyingNormal[1] * 0.5;
    vec3 ep2 = op2 + varyingNormal[2] * 0.5;

    vec3 newPoint1 = (op0 + op1 + op2) / 3.0;
    vec3 newPoint2 = (ep0 + ep1 + ep2) / 3.0;

    gl_Position = projMatrix * vec4(newPoint1, 1.0);
    EmitVertex();
    gl_Position = projMatrix * vec4(newPoint2, 1.0);
    EmitVertex();
    EndPrimitive();
}
)glsl";

const char* fragmentShader = R"glsl(
#version 430
out vec4 color;
void main()
{
    color = vec4(1.0, 1.0, 1.0, 1.0);
}
)glsl";

// render a explosion torus
int main(int argc, char const *argv[])
{
    Utils::Renderer renderer("03ChangePrimitiveType");
    Utils::Shader shader(vertexShader, fragmentShader, geometryShader);

    // vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // model and vbo
    Utils::Torus torus(1.0f, 3.0f);
    std::vector<int> indices = torus.getIndices();
    std::vector<glm::vec3> vertices = torus.getVertices();
    std::vector<glm::vec2> texCoords = torus.getTexCoords();
    std::vector<glm::vec3> normals = torus.getNormals();
    GLuint vbo[4] {};
    glGenBuffers(4, vbo);
    // indices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[0]);
    // vertices
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    // texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    // normals
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    auto display = [&](GLFWwindow* pWindow, float currentTime) -> void {
        glm::mat4 mMat(1.0f);
        glm::mat4 vMat = glm::lookAt(Utils::Renderer::getEyeLocation(pWindow), Utils::Renderer::getObjectLocation(pWindow), Utils::Renderer::getUpVector(pWindow));
        glm::mat4 pMat = Utils::Renderer::getProjMatrix(pWindow);
        
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        
        renderer.drawAxises();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        shader.use();
        shader.setMat4("mvMatrix", vMat * mMat);
        shader.setMat4("projMatrix", pMat);
        shader.setMat4("normMatrix", glm::transpose(glm::inverse(vMat * mMat)));
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
        Utils::checkOpenGLError();
    };
    renderer.setDisplayCallback(display);
    renderer.run();
    return 0;
}
