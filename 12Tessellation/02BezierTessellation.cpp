#include <Shader.h>
#include <Utils.h>
#include <Renderer.h>

const char* vertexShader = R"glsl(
#version 430
uniform mat4 mvp_matrix;
layout (binding = 0) uniform sampler2D texSamp;
out vec2 texCoord;
void main() {
    const vec4 vertices[] = vec4[] (
        vec4(-1.0, 0.5, -1.0, 1.0), vec4(-0.5, 0.5, -1.0, 1.0),
        vec4( 0.5, 0.5, -1.0, 1.0), vec4( 1.0, 0.5, -1.0, 1.0),
        
        vec4(-1.0, 0.0, -0.5, 1.0), vec4(-0.5, 0.0, -0.5, 1.0),
        vec4( 0.5, 0.0, -0.5, 1.0), vec4( 1.0, 0.0, -0.5, 1.0),
        
        vec4(-1.0, 0.0,  0.5, 1.0), vec4(-0.5, 0.0,  0.5, 1.0),
        vec4( 0.5, 0.0,  0.5, 1.0), vec4( 1.0, 0.0,  0.5, 1.0),

        vec4(-1.0, -0.5, 1.0, 1.0), vec4(-0.5, 0.3, 1.0, 1.0),
        vec4( 0.5,  0.3, 1.0, 1.0), vec4( 1.0, 0.3, 1.0, 1.0));
    // calculate texture coordinates, from [-1, 1] to [0, 1]
    texCoord = vec2((vertices[gl_VertexID].x + 1.0) / 2.0, (vertices[gl_VertexID].z + 1.0) / 2.0);
    gl_Position = vertices[gl_VertexID];
}
)glsl";

const char* tessCtrlShader = R"glsl(
#version 430
uniform mat4 mvp_matrix;
layout (binding = 0) uniform sampler2D texSamp;
layout (vertices = 16) out; // every patch has 16 control points

in vec2 texCoord[];
out vec2 texCoordTCSout[];
void main()
{
    int TL = 32;
    if (gl_InvocationID == 0)
    {
        // the tessellation level only need to be specified for once, so specify them only in call of 0
        gl_TessLevelOuter[0] = TL;
        gl_TessLevelOuter[1] = TL;
        gl_TessLevelOuter[2] = TL;
        gl_TessLevelOuter[3] = TL;
        gl_TessLevelInner[0] = TL;
        gl_TessLevelInner[1] = TL;
    }
    // pass texture coordinates and control points to tessellation evaluation shader
    texCoordTCSout[gl_InvocationID] = texCoord[gl_InvocationID]; // texture coordinates
    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position; // control points
}
)glsl";

const char* tessEvalShader = R"glsl(
#version 430
uniform mat4 mvp_matrix;
layout (binding = 0) uniform sampler2D texSamp;
layout (quads, equal_spacing, ccw) in;

in vec2 texCoordTCSout[];
out vec2 texCoordTESout;
void main()
{
    vec3 p00 = (gl_in[0].gl_Position).xyz;
    vec3 p10 = (gl_in[1].gl_Position).xyz;
    vec3 p20 = (gl_in[2].gl_Position).xyz;
    vec3 p30 = (gl_in[3].gl_Position).xyz;
    vec3 p01 = (gl_in[4].gl_Position).xyz;
    vec3 p11 = (gl_in[5].gl_Position).xyz;
    vec3 p21 = (gl_in[6].gl_Position).xyz;
    vec3 p31 = (gl_in[7].gl_Position).xyz;
    vec3 p02 = (gl_in[8].gl_Position).xyz;
    vec3 p12 = (gl_in[9].gl_Position).xyz;
    vec3 p22 = (gl_in[10].gl_Position).xyz;
    vec3 p32 = (gl_in[11].gl_Position).xyz;
    vec3 p03 = (gl_in[12].gl_Position).xyz;
    vec3 p13 = (gl_in[13].gl_Position).xyz;
    vec3 p23 = (gl_in[14].gl_Position).xyz;
    vec3 p33 = (gl_in[15].gl_Position).xyz;

    // gl_TessCoord save the grid coordinate of tessellation, in range [0.0, 1.0]
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;

    // cubic Bezier function
    float bu0 = (1.0-u) * (1.0-u) * (1.0-u);
    float bu1 = 3.0 * u * (1.0-u) * (1.0-u);
    float bu2 = 3.0 * u * u * (1.0-u);
    float bu3 = u * u * u;
    float bv0 = (1.0-v) * (1.0-v) * (1.0-v);
    float bv1 = 3.0 * v * (1.0-v) * (1.0-v);
    float bv2 = 3.0 * v * v * (1.0-v);
    float bv3 = v * v * v;
    vec3 outputPosition = 
        bu0 * (bv0*p00 + bv1*p01 + bv2*p02 + bv3*p03) +
        bu1 * (bv0*p10 + bv1*p11 + bv2*p12 + bv3*p13) +
        bu2 * (bv0*p20 + bv1*p21 + bv2*p22 + bv3*p23) +
        bu3 * (bv0*p30 + bv1*p31 + bv2*p32 + bv3*p33);
    gl_Position = mvp_matrix * vec4(outputPosition, 1.0f);

    // output texture coordinates
    vec2 tc1 = mix(texCoordTCSout[0], texCoordTCSout[3], gl_TessCoord.x);
    vec2 tc2 = mix(texCoordTCSout[12], texCoordTCSout[15], gl_TessCoord.x);
    vec2 tc = mix(tc2, tc1, gl_TessCoord.y);
    texCoordTESout = tc;
}
)glsl";

const char* fragmentShader = R"glsl(
#version 430
uniform mat4 mvp_matrix;
layout (binding = 0) uniform sampler2D texSamp;

in vec2 texCoordTESout;
out vec4 color;
void main()
{
    color = texture(texSamp, texCoordTESout);
}
)glsl";


int main(int argc, char const *argv[])
{
    Utils::Renderer renderer("02BezierTessellation");
    Utils::Shader tessShader(vertexShader, tessCtrlShader, tessEvalShader, fragmentShader);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
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
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_BACK);
        // glFrontFace(GL_CCW);

        tessShader.use();
        tessShader.setMat4("mvp_matrix", pMat * vMat * mMat);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glPatchParameteri(GL_PATCH_VERTICES, 16);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glBindVertexArray(vao);
        glDrawArrays(GL_PATCHES, 0, 16);
        Utils::checkOpenGLError();
    };
    renderer.setDisplayCallback(display);
    renderer.run();
    return 0;
}
