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

out vec2 texCoord;
out vec3 varyingNormal;
void main()
{
    gl_Position = mvMatrix * vec4(vertexPos, 1.0f);
    texCoord = textureCoord;
    varyingNormal = (normMatrix * vec4(vertexNormal, 1.0f)).xyz;
}
)glsl";

const char* geometryShader = R"glsl(
#version 430
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 mvMatrix;
uniform mat4 normMatrix;
uniform mat4 projMatrix;

in vec2 texCoord[];
in vec3 varyingNormal[];
out vec2 texCoordG;
void main()
{
    // delete one primitive out of every 7 primitives
    if (mod(gl_PrimitiveIDIn, 7) != 0)
    {
        for (int i = 0; i < 3; i++)
        {
            gl_Position = projMatrix * gl_in[i].gl_Position;
            texCoordG = texCoord[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}
)glsl";

const char* fragmentShader = R"glsl(
#version 430
layout (binding = 0) uniform sampler2D samp;
in vec2 texCoordG;
out vec4 color;
void main()
{
    color = texture(samp, texCoordG);
}
)glsl";

// render a explosion torus
int main(int argc, char const *argv[])
{
    Utils::Renderer renderer("02DeletePrimitive");
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

    // texture
    GLuint textureId = Utils::loadTexture("wood.png");

    auto display = [&](GLFWwindow* pWindow, float currentTime) -> void {
        glm::mat4 mMat(1.0f);
        glm::mat4 vMat = glm::lookAt(Utils::Renderer::getEyeLocation(pWindow), Utils::Renderer::getObjectLocation(pWindow), Utils::Renderer::getUpVector(pWindow));
        glm::mat4 pMat = Utils::Renderer::getProjMatrix(pWindow);
        
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        
        renderer.drawAxises();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        shader.use();
        shader.setMat4("mvMatrix", vMat * mMat);
        shader.setMat4("projMatrix", pMat);
        shader.setMat4("normMatrix", glm::transpose(glm::inverse(vMat * mMat)));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, GLsizei(indices.size()), GL_UNSIGNED_INT, 0);
        Utils::checkOpenGLError();
    };
    renderer.setDisplayCallback(display);
    renderer.run();
    return 0;
}
