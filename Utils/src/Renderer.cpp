#include <Renderer.h>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <utility>
#include "Utils.h"

namespace Utils
{

// predefined shader sources
// ================================ xyz axises ================================
const char* axisesVertexShader = R"glsl(
#version 430
layout (location = 0) in vec3 position; // the vertex array buffer
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
uniform float axisLength;
out vec4 axisColor;
void main(void)
{
    gl_Position = projMatrix * mvMatrix * vec4(position, 1.0);
    axisColor = vec4(position, 1.0) / (axisLength * 2.0); // value in [-0.25, 0.25]
    if (axisColor.x != 0.0)
    {
        axisColor.x += 0.75;
    }
    if (axisColor.y != 0.0)
    {
        axisColor.y += 0.75;
    }
    if (axisColor.z != 0.0)
    {
        axisColor.z += 0.75;
    }
}
)glsl";

const char* axisesFragmentShader = R"glsl(
#version 430
in vec4 axisColor;
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
out vec4 color;
void main(void)
{
    color = axisColor;
}
)glsl";


// ================================ pure color ================================ 
const char* pureColorVertexShader = R"glsl(
#version 430
layout (location = 0) in vec3 position; // the vertex array buffer
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
uniform vec4 inputColor;
void main(void)
{
    gl_Position = projMatrix * mvMatrix * vec4(position, 1.0);
}
)glsl";

const char* pureColorFragmentShader = R"glsl(
#version 430
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
uniform vec4 inputColor; // render to the input color
out vec4 color;
void main(void)
{
    color = inputColor;
}
)glsl";

// ================================ varying color ================================ 
const char* varyingColorVertexShader = R"glsl(
#version 430
layout (location = 0) in vec3 position; // the vertex array buffer
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
out vec4 varyingColor;
void main(void)
{
    gl_Position = projMatrix * mvMatrix * vec4(position, 1.0);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}
)glsl";

const char* varyingColorFragmentShader = R"glsl(
#version 430
in vec4 varyingColor;
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
out vec4 color;
void main(void)
{
    color = varyingColor; // color varies with position
}
)glsl";

// ================================ texture ================================ 
const char* textureVertexShader = R"glsl(
#version 430
layout (location = 0) in vec3 position;     // the model vertex buffer
layout (location = 1) in vec2 texureCoord;  // the texture vertex buffer
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
layout (binding = 0) uniform sampler2D samp; // always texture unit 0
out vec2 tc; // texture coordinates output
void main(void)
{
    gl_Position = projMatrix * mvMatrix * vec4(position, 1.0);
    tc = texureCoord;
}
)glsl";

const char* textureFragmentShader = R"glsl(
#version 430
in vec2 tc; // texture coordinates input
uniform mat4 mvMatrix;
uniform mat4 projMatrix;
layout (binding = 0) uniform sampler2D samp; // always texture unit 0
out vec4 color;
void main()
{
    color = texture(samp, tc);
}
)glsl";

// ================================ Gouraud shading with lighting & material & texture ================================ 
const char* GouraudLightingMaterialTextureVertexShader = R"glsl(
#version 430
// different max light numbers, must be same as the number in the Renderer class !
#define MAX_POINT_LIGHT_SIZE 10
#define MAX_DIRECTIONAL_LIGHT_SIZE 10
#define MAX_SPOT_LIGHT_SIZE 10
struct DirectionalLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 direction; // in view space
};
struct PointLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 location; // in view space
    // for attenuation factor
    float constant;
    float linear;
    float quadratic;
};
struct SpotLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 location; // in view space
    vec3 direction; // in view space
    float cutOffAngle;
    float strengthFactorExponent;
};
struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
layout (location = 0) in vec3 vertexPos;        // vertex buffer
layout (location = 1) in vec2 texureCoord;      // texture coordinates
layout (location = 2) in vec3 vertexNormal;     // normals of vertices
// texture sampler
layout (binding = 0) uniform sampler2D samp;
// lights
uniform vec4 globalAmbient;
uniform uint directionalLightsSize;
uniform uint pointLightsSize;
uniform uint spotLightsSize;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_SIZE];
uniform PointLight pointLights[MAX_POINT_LIGHT_SIZE];
uniform SpotLight spotLights[MAX_SPOT_LIGHT_SIZE];
// material
uniform Material material;
// matrices
uniform mat4 mvMatrix;      // model-view matrix
uniform mat4 projMatrix;    // projection matrix
uniform mat4 normMatrix;    // for transformation of normal vector
uniform int flatShading;    // flat shading or not
// material and texture weight
uniform float materialWeight;
uniform float textureWeight;

out vec3 ambient;
out vec3 diffuse;
out vec3 materialSpecular;
out vec3 textureSpecular;

flat out vec3 flatAmbient;
flat out vec3 flatDiffuse;
flat out vec3 flatMaterialSpecular;
flat out vec3 flatTextureSpecular;

out vec2 tc;

/*
  light source
  \
   L  N  R   ___ eye
    \ | /___/ V
_____\|/____________
      P
*/

void calculateDirectionalLight(DirectionalLight light, vec3 P, vec3 N)
{
    // light vector (from vertex to light source) in view space
    vec3 L = normalize(-light.direction);
    // visual vector (from vertex to eye), equals to the nagative vertex position in view space
    vec3 V = normalize(-P);
    // reflect vector
    vec3 R = reflect(-L, N);
    // the ADS weight of vertex
    ambient += light.ambient.xyz;
    diffuse += light.diffuse.xyz * max(dot(N, L), 0.0);
    materialSpecular += light.specular.xyz * pow(max(dot(R, V), 0.0), material.shininess);
    // shininess will be always 1.0 for texture, is this proper?
    textureSpecular += light.specular.xyz * max(dot(R, V), 0.0);
}

void calculatePointLight(PointLight light, vec3 P, vec3 N)
{
    // light vector (from vertex to light source) in view space
    vec3 L = normalize(light.location - P);
    // visual vector (from vertex to eye) equals to the negative vertex position in view space
    vec3 V = normalize(-P);
    // reflect vector
    vec3 R = reflect(-L, N);
    // attenuation factor
    float distance = length(light.location - P);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    // the ADS weight of vertex
    ambient += light.ambient.xyz * attenuation;
    diffuse += light.diffuse.xyz * max(dot(N, L), 0.0) * attenuation;
    materialSpecular += light.specular.xyz * pow(max(dot(R, V), 0.0), material.shininess) * attenuation;
    // shininess will be always 1.0 for texture, is this proper?
    textureSpecular += light.specular.xyz * max(dot(R, V), 0.0) * attenuation;
}

void calculateSpotLight(SpotLight light, vec3 P, vec3 N)
{
    // light vector (from vertex to light source) in view space
    vec3 L = normalize(light.location - P);
    // visual vector (from vertex to eye)
    vec3 V = normalize(-P);
    // reflect vector
    vec3 R = reflect(-L, N);
    // the angle between visual vector and the center of spot light
    float cosOffAxisAngle = dot(-L, normalize(light.direction));
    // strength factor
    float strengthFactor = (cosOffAxisAngle > cos(light.cutOffAngle)) ? pow(cosOffAxisAngle, light.strengthFactorExponent) : 0;
    // attenuation factor: no attenuation for now, may be model it as point light
    // ADS weight of vertex
    ambient += light.ambient.xyz * strengthFactor;
    diffuse += light.diffuse.xyz * max(dot(N, L), 0.0) * strengthFactor;
    materialSpecular += light.specular.xyz * pow(max(dot(R, V), 0.0), material.shininess) * strengthFactor;
    // shininess will be always 1.0 for texture, is this proper?
    textureSpecular += light.specular.xyz * max(dot(R, V), 0.0) * strengthFactor;
}

void main()
{
    // initialization
    ambient = vec3(0.0, 0.0, 0.0);
    diffuse = vec3(0.0, 0.0, 0.0);
    materialSpecular = vec3(0.0, 0.0, 0.0);
    textureSpecular = vec3(0.0, 0.0, 0.0);

    // vertex position in view space
    vec4 P = mvMatrix * vec4(vertexPos, 1.0);
    // normal vector in view space
    vec3 N = normalize((normMatrix * vec4(vertexNormal, 1.0)).xyz);

    // global ambient
    ambient += globalAmbient.xyz;
    // directional lights
    for (uint i = 0; i < directionalLightsSize; i++)
    {
        calculateDirectionalLight(directionalLights[i], P.xyz, N);
    }
    // point lights
    for (uint i = 0; i < pointLightsSize; i++)
    {
        calculatePointLight(pointLights[i], P.xyz, N);
    }
    // spot lights
    for (uint i = 0; i < spotLightsSize; i++)
    {
        calculateSpotLight(spotLights[i], P.xyz, N);
    }
    
    // for flat shading
    flatAmbient = ambient;
    flatDiffuse = diffuse;
    flatMaterialSpecular = materialSpecular;
    flatTextureSpecular = textureSpecular;

    // position
    gl_Position = projMatrix * mvMatrix * vec4(vertexPos, 1.0);

    // texture coordinates
    tc = texureCoord;
}
)glsl";

const char* GouraudLightingMaterialTextureFragmentShader = R"glsl(
#version 430
// different max light numbers, must be same as the number in the Renderer class !
#define MAX_POINT_LIGHT_SIZE 10
#define MAX_DIRECTIONAL_LIGHT_SIZE 10
#define MAX_SPOT_LIGHT_SIZE 10
struct DirectionalLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 direction; // in view space
};
struct PointLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 location; // in view space
    // for attenuation factor
    float constant;
    float linear;
    float quadratic;
};
struct SpotLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 location; // in view space
    vec3 direction; // in view space
    float cutOffAngle;
    float strengthFactorExponent;
};
struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
layout (location = 0) in vec3 vertexPos;        // vertex buffer
layout (location = 1) in vec2 texureCoord;      // texture coordinates
layout (location = 2) in vec3 vertexNormal;     // normals of vertices
// texture sampler
layout (binding = 0) uniform sampler2D samp;
// lights
uniform vec4 globalAmbient;
uniform uint directionalLightsSize;
uniform uint pointLightsSize;
uniform uint spotLightsSize;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_SIZE];
uniform PointLight pointLights[MAX_POINT_LIGHT_SIZE];
uniform SpotLight spotLights[MAX_SPOT_LIGHT_SIZE];
// material
uniform Material material;
// matrices
uniform mat4 mvMatrix;      // model-view matrix
uniform mat4 projMatrix;    // projection matrix
uniform mat4 normMatrix;    // for transformation of normal vector
uniform int flatShading;    // flat shading or not
// material and texture weight
uniform float materialWeight;
uniform float textureWeight;

in vec3 ambient;
in vec3 diffuse;
in vec3 materialSpecular;
in vec3 textureSpecular;
flat in vec3 flatAmbient;
flat in vec3 flatDiffuse;
flat in vec3 flatMaterialSpecular;
flat in vec3 flatTextureSpecular;
in vec2 tc;

out vec4 fragColor;

void main()
{
    fragColor = vec4(0.0, 0.0, 0.0, 0.0);
    if (flatShading == 1) // flat shading
    {
        if (textureWeight != 0.0)
        {
            fragColor += textureWeight * (texture(samp, tc) * vec4(0.3 * flatAmbient + 0.4 * flatDiffuse + 0.4 * flatTextureSpecular, 1.0));
        }
        if (materialWeight != 0.0)
        {
            fragColor += materialWeight * vec4(material.ambient.xyz * flatAmbient + material.diffuse.xyz * flatDiffuse + material.specular.xyz * flatMaterialSpecular, 1.0);
        }
    }
    else // Gouraud shading
    {
        if (textureWeight != 0.0)
        {
            fragColor += textureWeight * (texture(samp, tc) * vec4(0.3 * ambient + 0.4 * diffuse + 0.4 * textureSpecular, 1.0));
        }
        if (materialWeight != 0.0)
        {
            fragColor += materialWeight * vec4(material.ambient.xyz * ambient + material.diffuse.xyz * diffuse + material.specular.xyz * materialSpecular, 1.0);
        }
    }
}
)glsl";

// ================================ Phong shading with lighting & material & texture ================================ 
const char* PhongLightingMaterialTextureVertexShader = R"glsl(
#version 430
// different max light numbers, must be same as the number in the Renderer class !
#define MAX_POINT_LIGHT_SIZE 10
#define MAX_DIRECTIONAL_LIGHT_SIZE 10
#define MAX_SPOT_LIGHT_SIZE 10
struct DirectionalLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 direction; // in view space
};
struct PointLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 location; // in view space
    // for attenuation factor
    float constant;
    float linear;
    float quadratic;
};
struct SpotLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 location; // in view space
    vec3 direction; // in view space
    float cutOffAngle;
    float strengthFactorExponent;
};
struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
layout (location = 0) in vec3 vertexPos;        // vertex buffer
layout (location = 1) in vec2 texureCoord;      // texture coordinates
layout (location = 2) in vec3 vertexNormal;     // normals of vertices
// texture sampler
layout (binding = 0) uniform sampler2D samp;
// lights
uniform vec4 globalAmbient;
uniform uint directionalLightsSize;
uniform uint pointLightsSize;
uniform uint spotLightsSize;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_SIZE];
uniform PointLight pointLights[MAX_POINT_LIGHT_SIZE];
uniform SpotLight spotLights[MAX_SPOT_LIGHT_SIZE];
// material
uniform Material material;
// matrices
uniform mat4 mvMatrix;      // model-view matrix
uniform mat4 projMatrix;    // projection matrix
uniform mat4 normMatrix;    // for transformation of normal vector
// material and texture weight
uniform float materialWeight;
uniform float textureWeight;

out vec3 varyingNormal;
out vec3 varyingVertexPos;
// varying light direction (from vertex to light source) for different light sources (in view space)
out vec3 varyingPointLightDirections[MAX_POINT_LIGHT_SIZE];
out vec3 varyingSpotLightDirections[MAX_SPOT_LIGHT_SIZE];
out vec2 tc;

void main()
{
    varyingVertexPos = (mvMatrix * vec4(vertexPos, 1.0)).xyz;
    varyingNormal = (normMatrix * vec4(vertexNormal, 1.0)).xyz;
    for (uint i = 0; i < pointLightsSize; i++)
    {
        varyingPointLightDirections[i] = pointLights[i].location - varyingVertexPos;
    }
    for (uint i = 0; i < spotLightsSize; i++)
    {
        varyingSpotLightDirections[i] = spotLights[i].location - varyingVertexPos;
    }
    gl_Position = projMatrix * mvMatrix * vec4(vertexPos, 1.0);
    // texture coordinates
    tc = texureCoord;
}
)glsl";

const char* PhongLightingMaterialTextureFragmentShader = R"glsl(
#version 430
// different max light numbers, must be same as the number in the Renderer class !
#define MAX_POINT_LIGHT_SIZE 10
#define MAX_DIRECTIONAL_LIGHT_SIZE 10
#define MAX_SPOT_LIGHT_SIZE 10
struct DirectionalLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 direction; // in view space
};
struct PointLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 location; // in view space
    // for attenuation factor
    float constant;
    float linear;
    float quadratic;
};
struct SpotLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 location; // in view space
    vec3 direction; // in view space
    float cutOffAngle;
    float strengthFactorExponent;
};
struct Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};
layout (location = 0) in vec3 vertexPos;        // vertex buffer
layout (location = 1) in vec2 texureCoord;      // texture coordinates
layout (location = 2) in vec3 vertexNormal;     // normals of vertices
// texture sampler
layout (binding = 0) uniform sampler2D samp;
// lights
uniform vec4 globalAmbient;
uniform uint directionalLightsSize;
uniform uint pointLightsSize;
uniform uint spotLightsSize;
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHT_SIZE];
uniform PointLight pointLights[MAX_POINT_LIGHT_SIZE];
uniform SpotLight spotLights[MAX_SPOT_LIGHT_SIZE];
// material
uniform Material material;
// matrices
uniform mat4 mvMatrix;      // model-view matrix
uniform mat4 projMatrix;    // projection matrix
uniform mat4 normMatrix;    // for transformation of normal vector
// material and texture weight
uniform float materialWeight;
uniform float textureWeight;

in vec3 varyingNormal;
in vec3 varyingVertexPos;
// varying light direction (from vertex to light source) for different light sources (in view space)
in vec3 varyingPointLightDirections[MAX_POINT_LIGHT_SIZE];
in vec3 varyingSpotLightDirections[MAX_SPOT_LIGHT_SIZE];
in vec2 tc;

out vec4 fragColor;

vec3 ambient;
vec3 diffuse;
vec3 materialSpecular;
vec3 textureSpecular;

void calculateDirectionalLight(DirectionalLight light, vec3 P, vec3 N)
{
    // light vector (from vertex to light source) in view space
    vec3 L = normalize(-light.direction);
    // visual vector (from vertex to eye), equals to the nagative vertex position in view space
    vec3 V = normalize(-P);
    // reflect vector
    vec3 R = reflect(-L, N);
    // the ADS weight of vertex
    ambient += light.ambient.xyz;
    diffuse += light.diffuse.xyz * max(dot(N, L), 0.0);
    materialSpecular += light.specular.xyz * pow(max(dot(R, V), 0.0), material.shininess);
    // shininess will be always 1.0 for texture, is this proper?
    textureSpecular += light.specular.xyz * max(dot(R, V), 0.0);
}

void calculatePointLight(PointLight light, uint index, vec3 P, vec3 N)
{
    // light vector (from vertex to light source) in view space
    vec3 L = normalize(varyingPointLightDirections[index]);
    // visual vector (from vertex to eye) equals to the negative vertex position in view space
    vec3 V = normalize(-P);
    // reflect vector
    vec3 R = reflect(-L, N);
    // attenuation factor
    float distance = length(light.location - P);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    // the ADS weight of vertex
    ambient += light.ambient.xyz * attenuation;
    diffuse += light.diffuse.xyz * max(dot(N, L), 0.0) * attenuation;
    materialSpecular += light.specular.xyz * pow(max(dot(R, V), 0.0), material.shininess) * attenuation;
    // shininess will be always 1.0 for texture, is this proper?
    textureSpecular += light.specular.xyz * max(dot(R, V), 0.0) * attenuation;
}

void calculateSpotLight(SpotLight light, uint index, vec3 P, vec3 N)
{
    // light vector (from vertex to light source) in view space
    vec3 L = normalize(varyingSpotLightDirections[index]);
    // visual vector (from vertex to eye)
    vec3 V = normalize(-P);
    // reflect vector
    vec3 R = reflect(-L, N);
    // the angle between visual vector and the center of spot light
    float cosOffAxisAngle = dot(-L, normalize(light.direction));
    // strength factor
    float strengthFactor = (cosOffAxisAngle > cos(light.cutOffAngle)) ? pow(cosOffAxisAngle, light.strengthFactorExponent) : 0;
    // attenuation factor: no attenuation for now, may be model it as point light
    // ADS weight of vertex
    ambient += light.ambient.xyz * strengthFactor;
    diffuse += light.diffuse.xyz * max(dot(N, L), 0.0) * strengthFactor;
    materialSpecular += light.specular.xyz * pow(max(dot(R, V), 0.0), material.shininess) * strengthFactor;
    // shininess will be always 1.0 for texture, is this proper?
    textureSpecular += light.specular.xyz * max(dot(R, V), 0.0) * strengthFactor;
}

void main()
{
    // initialization
    ambient = vec3(0.0, 0.0, 0.0);
    diffuse = vec3(0.0, 0.0, 0.0);
    materialSpecular = vec3(0.0, 0.0, 0.0);
    textureSpecular = vec3(0.0, 0.0, 0.0);

    // global ambient
    ambient += globalAmbient.xyz;
    // directional lights
    for (uint i = 0; i < directionalLightsSize; i++)
    {
        calculateDirectionalLight(directionalLights[i], varyingVertexPos, varyingNormal);
    }
    // point lights
    for (uint i = 0; i < pointLightsSize; i++)
    {
        calculatePointLight(pointLights[i], i, varyingVertexPos, varyingNormal);
    }
    // spot lights
    for (uint i = 0; i < spotLightsSize; i++)
    {
        calculateSpotLight(spotLights[i], i, varyingVertexPos, varyingNormal);
    }
    // result
    fragColor = vec4(0.0, 0.0, 0.0, 0.0);
    if (textureWeight != 0.0)
    {
        fragColor += textureWeight * (texture(samp, tc) * vec4(0.3 * ambient + 0.4 * diffuse + 0.4 * textureSpecular, 1.0));
    }
    if (materialWeight != 0.0)
    {
        fragColor += materialWeight * vec4(material.ambient.xyz * ambient + material.diffuse.xyz * diffuse + material.specular.xyz * materialSpecular, 1.0);
    }
}
)glsl";

// ======================================================= Renderer ==============================================
Renderer::Renderer(const char* windowTitle, int width, int height, float axisLength)
    : m_AxisLength(axisLength)
{
    // init glfw
    if (!glfwInit())
    {
        glfwTerminate();
        std::exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // create window
    m_pWindow = glfwCreateWindow(width, height, windowTitle, NULL, NULL);
    if (!m_pWindow)
    {
        glfwTerminate();
        std::exit(-1);
    }
    glfwMakeContextCurrent(m_pWindow);
    // init glad
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        glfwTerminate();
        std::exit(-1);
    }
    glfwSwapInterval(1);

    // vao, vbos are added when call addModel
    m_VaoVec.resize(1);
    glGenVertexArrays(1, &m_VaoVec[0]);
    glBindVertexArray(m_VaoVec[0]);

    // shaders
    m_AxisesShaderProgram = createShaderProgramFromSource(axisesVertexShader, axisesFragmentShader);
    m_PureColorShaderProgram = createShaderProgramFromSource(pureColorVertexShader, pureColorFragmentShader);
    m_VaryingColorShaderProgram = createShaderProgramFromSource(varyingColorVertexShader, varyingColorFragmentShader);
    m_TextureShaderProgram = createShaderProgramFromSource(textureVertexShader, textureFragmentShader);
    m_GouraudLightingMaterialTextureShderProgram = createShaderProgramFromSource(GouraudLightingMaterialTextureVertexShader, GouraudLightingMaterialTextureFragmentShader);
    m_PhongLightingMaterialTextureShaderProgram = createShaderProgramFromSource(PhongLightingMaterialTextureVertexShader, PhongLightingMaterialTextureFragmentShader);

    // init window attributes
    s_WindowAttrs.insert(std::make_pair(m_pWindow, WindowAttributes{}));

    // projection matrix
    // recalculate the perspective projection matrix when window size changes
    auto calculatePerspectiveMatrix = [](GLFWwindow* pWindow, int newWidth, int newHeight) -> void
    {
        if (newWidth != 0 && newHeight != 0) // when minimization
        {
            float aspect = float(newWidth) / float(newHeight);
            glViewport(0, 0, newWidth, newHeight); // set the screen area associated with the frame buffer (aka the view port).
            getProjMatrix(pWindow) = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
        }
    };
    glfwSetWindowSizeCallback(m_pWindow, calculatePerspectiveMatrix);
    // build the perspective proejction matrix
    glfwGetFramebufferSize(m_pWindow, &width, &height); // screen width and height
    calculatePerspectiveMatrix(m_pWindow, width, height);

    // scroll call back, change view matrix (only the eye position)
    auto scrollCallback = [](GLFWwindow* pWindow, double xoffset, double yoffset) -> void
    {
        glm::vec3& eyeLocation = getEyeLocation(pWindow);
        glm::vec3& objectLocation = getObjectLocation(pWindow);

        int offset = int(yoffset);
        while (offset >= 1)
        {
            eyeLocation = (eyeLocation - objectLocation) * 0.8f + objectLocation;
            offset -= 1;
        }
        while (offset <= -1)
        {
            eyeLocation = (eyeLocation - objectLocation) * 1.25f + objectLocation;
            offset += 1;
        }
    };
    glfwSetScrollCallback(m_pWindow, scrollCallback);

    // mouse button input callback, combine with cursor position callback to change view matrix
    auto mouseButtonInputCallback = [](GLFWwindow* pWindow, int button, int action, int mods) -> void
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        {
            getHoldLeftMouseButton(pWindow) = true;
        }
        else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        {
            getHoldLeftMouseButton(pWindow) = false;
        }
    };
    glfwSetMouseButtonCallback(m_pWindow, mouseButtonInputCallback);

    // cursor position callback, change view matrix (the eye position and the up direction) when left button is holding.
    // view matrix will be updated in every frame for efficiency
    auto cursorPositionCallback = [](GLFWwindow* pWindow, double xpos, double ypos)
    {
        getCursorPosX(pWindow) = int(xpos);
        getCursorPosY(pWindow) = int(ypos);
    };
    glfwSetCursorPosCallback(m_pWindow, cursorPositionCallback);

    // axises
    float axisesCoords[18] = {
        -m_AxisLength, 0.0f, 0.0f, m_AxisLength, 0.0f, 0.0f,
        0.0f, -m_AxisLength, 0.0f, 0.0f, m_AxisLength, 0.0f,
        0.0f, 0.0f, -m_AxisLength, 0.0f, 0.0f, m_AxisLength
    };
    glGenBuffers(1, &m_AxisesVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_AxisesVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisesCoords), axisesCoords, GL_STATIC_DRAW);
}

Renderer::~Renderer()
{
    glfwDestroyWindow(m_pWindow);
    glfwTerminate();
}

void Renderer::run()
{
    checkForModelAttributes();
    // render loop
    while (!glfwWindowShouldClose(m_pWindow))
    {
        updateViewArgsAccordingToCursorPos();
        display(float(glfwGetTime()));
        glfwSwapBuffers(m_pWindow);
        glfwPollEvents();
    }
}

// add model to render, return it's index
std::size_t Renderer::addModel(std::shared_ptr<Model> spModel, RenderStyle renderStyle)
{
    m_Models.push_back(ModelAttributes{});
    ModelAttributes& attr = m_Models.back();
    attr.spModel = spModel;
    attr.style = renderStyle;
    attr.indicesVbo = attr.verticesVbo = attr.texCoordVbo = attr.normalVbo = attr.sTanVbo = attr.tTanVbo = 0;

    if (spModel->supplyIndices() && spModel->supplyVertices())
    {
        // ensuring the assumptions we depend on are valid.
        static_assert(sizeof(glm::vec3) == sizeof(float) * 3);
        static_assert(sizeof(glm::vec2) == sizeof(float) * 2);

        std::vector<int> indices = spModel->getIndices();
        std::vector<glm::vec3> vertices = spModel->getVertices();
        glGenBuffers(1, &attr.indicesVbo);
        glGenBuffers(1, &attr.verticesVbo);
        attr.verticesCount = GLsizei(indices.size());
        // indices
        glBindBuffer(GL_ARRAY_BUFFER, attr.indicesVbo);
        glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(int), indices.data(), GL_STATIC_DRAW);
        // vertices
        glBindBuffer(GL_ARRAY_BUFFER, attr.verticesVbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        
        if (spModel->supplyTexCoords())
        {
            std::vector<glm::vec2> texCoords = spModel->getTexCoords();
            glGenBuffers(1, &attr.texCoordVbo);
            glBindBuffer(GL_ARRAY_BUFFER, attr.texCoordVbo);
            glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);
        }
        if (spModel->supplyNormals())
        {
            std::vector<glm::vec3> normals = spModel->getNormals();
            glGenBuffers(1, &attr.normalVbo);
            glBindBuffer(GL_ARRAY_BUFFER, attr.normalVbo);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
        }
        if (spModel->supplyTangents())
        {
            std::vector<glm::vec3> sTanVec = spModel->getSTangents();
            std::vector<glm::vec3> tTanVec = spModel->getTTangents();
            glGenBuffers(1, &attr.sTanVbo);
            glGenBuffers(1, &attr.tTanVbo);
            glBindBuffer(GL_ARRAY_BUFFER, attr.sTanVbo);
            glBindBuffer(GL_ARRAY_BUFFER, attr.tTanVbo);
            glBufferData(GL_ARRAY_BUFFER, sTanVec.size() * sizeof(glm::vec3), sTanVec.data(), GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, tTanVec.size() * sizeof(glm::vec3), tTanVec.data(), GL_STATIC_DRAW);
        }
    }
    else if (spModel->supplyVertices())
    {
        std::vector<float> vertices = spModel->getVerticesArray();
        attr.verticesCount = GLsizei(vertices.size() / 3);
        glGenBuffers(1, &attr.verticesVbo);
        glBindBuffer(GL_ARRAY_BUFFER, attr.verticesVbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        if (spModel->supplyTexCoords())
        {
            std::vector<float> texCoords = spModel->getTexCoordsArray();
            glGenBuffers(1, &attr.texCoordVbo);
            glBindBuffer(GL_ARRAY_BUFFER, attr.texCoordVbo);
            glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(float), texCoords.data(), GL_STATIC_DRAW);
        }
        if (spModel->supplyNormals())
        {
            std::vector<float> normals = spModel->getNormalsArray();
            glGenBuffers(1, &attr.normalVbo);
            glBindBuffer(GL_ARRAY_BUFFER, attr.normalVbo);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
        }
        if (spModel->supplyTangents())
        {
            std::vector<float> sTanVec = spModel->getSTangentsArray();
            std::vector<float> tTanVec = spModel->getTTangentsArray();
            glGenBuffers(1, &attr.sTanVbo);
            glGenBuffers(1, &attr.tTanVbo);
            glBindBuffer(GL_ARRAY_BUFFER, attr.sTanVbo);
            glBindBuffer(GL_ARRAY_BUFFER, attr.tTanVbo);
            glBufferData(GL_ARRAY_BUFFER, sTanVec.size() * sizeof(float), sTanVec.data(), GL_STATIC_DRAW);
            glBufferData(GL_ARRAY_BUFFER, tTanVec.size() * sizeof(float), tTanVec.data(), GL_STATIC_DRAW);
        }
    }
    return m_Models.size() - 1;
}

// add different kinds of lighting
void Renderer::setGlobalAmbientLight(glm::vec4 ambient)
{
    m_GlobalAmbient = ambient;
}
void Renderer::addDirectionalLight(const DirectionalLight& light)
{
    if (m_DirectionalLights.size() == MAX_DIRECTIONAL_LIGHT_SIZE)
    {
        Utils::Logger::globalLogger().warning("Directional light sources number exceed the limit of "s + std::to_string(MAX_DIRECTIONAL_LIGHT_SIZE) + "!");
    }
    else
    {
        m_DirectionalLights.push_back(light);
    }
}
void Renderer::addDirectionalLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 direction)
{
    if (m_DirectionalLights.size() == MAX_DIRECTIONAL_LIGHT_SIZE)
    {
        Utils::Logger::globalLogger().warning("Directional light sources number exceed the limit of "s + std::to_string(MAX_DIRECTIONAL_LIGHT_SIZE) + "!");
    }
    else
    {
        m_DirectionalLights.emplace_back(ambient, diffuse, specular, direction);
    }
}
void Renderer::addPointLight(const PointLight& light)
{
    if (m_PointLights.size() == MAX_POINT_LIGHT_SIZE)
    {
        Utils::Logger::globalLogger().warning("Point light sources number exceed the limit of "s + std::to_string(MAX_POINT_LIGHT_SIZE) + "!");
    }
    else
    {
        m_PointLights.push_back(light);
    }
}
void Renderer::addPointLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, float constant, float linear, float quadratic)
{
    if (m_PointLights.size() == MAX_POINT_LIGHT_SIZE)
    {
        Utils::Logger::globalLogger().warning("Point light sources number exceed the limit of "s + std::to_string(MAX_POINT_LIGHT_SIZE) + "!");
    }
    else
    {
        m_PointLights.emplace_back(ambient, diffuse, specular, location, constant, linear, quadratic);
    }
}
void Renderer::addSpotLight(const SpotLight& light)
{
    if (m_SpotLights.size() == MAX_SPOT_LIGHT_SIZE)
    {
        Utils::Logger::globalLogger().warning("Spot light sources number exceed the limit of "s + std::to_string(MAX_SPOT_LIGHT_SIZE) + "!");
    }
    else
    {
        m_SpotLights.push_back(light);
    }
}
void Renderer::addSpotLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, glm::vec3 direction, float cutoff, float exponent)
{
    if (m_SpotLights.size() == MAX_SPOT_LIGHT_SIZE)
    {
        Utils::Logger::globalLogger().warning("Spot light sources number exceed the limit of "s + std::to_string(MAX_SPOT_LIGHT_SIZE) + "!");
    }
    else
    {
        m_SpotLights.emplace_back(ambient, diffuse, specular, location, direction, cutoff, exponent);
    }
}

// set face culling attributes, defualt to true/GL_BACK/GL_CCW
void Renderer::setFaceCullingAttribute(bool enable, GLenum mode, GLenum front)
{
    m_bEnableCullFace = enable;
    m_FaceCullingMode = mode;
    m_FrontFace = front;
} 

// set model attributes
// set rotatoin attributes, default to false
void Renderer::setModelRotation(std::size_t modelIndex, glm::vec3 rotationAxis, float rotationRate)
{
    assert(modelIndex < m_Models.size());
    m_Models[modelIndex].bRotate = true;
    m_Models[modelIndex].rotationAxis = rotationAxis;
    m_Models[modelIndex].rotationRate = rotationRate;
}

// set render style
void Renderer::setRenderStyle(std::size_t modelIndex, RenderStyle renderStyle)
{
    assert(modelIndex < m_Models.size());
    m_Models[modelIndex].style = renderStyle;
}

// set color to render, for PureColorPoints/PureColorLineStrip/PureColorTriangles style, default to 
void Renderer::setColor(std::size_t modelIndex, glm::vec4 color)
{
    assert(modelIndex < m_Models.size());
    m_Models[modelIndex].color = color;
}

// set texture for model, just for SpecificTexture style
void Renderer::setTexture(std::size_t modelIndex, const char* textureImagePath, float weight, bool doMipmapping, bool doAnisotropicFiltering)
{
    assert(modelIndex < m_Models.size());
    m_Models[modelIndex].texture = loadTexture(textureImagePath);
    m_Models[modelIndex].textureWeight = weight;
    m_Models[modelIndex].doMipmapping = doMipmapping;
    m_Models[modelIndex].doAnisotropicFiltering = doAnisotropicFiltering;
}
void Renderer::setTexture(std::size_t modelIndex, GLuint textureId, float weight, bool doMipmapping, bool doAnisotropicFiltering)
{
    assert(modelIndex < m_Models.size());
    m_Models[modelIndex].texture = textureId;
    m_Models[modelIndex].textureWeight = weight;
    m_Models[modelIndex].doMipmapping = doMipmapping;
    m_Models[modelIndex].doAnisotropicFiltering = doAnisotropicFiltering;
}

// set material for model, for LightingMaterial style
void Renderer::setMaterial(std::size_t modelIndex, const Material& material, float weight)
{
    assert(modelIndex < m_Models.size());
    m_Models[modelIndex].spMaterial = std::make_unique<Material>(material);
    m_Models[modelIndex].materialWeight = weight;
}

// set lighting mode of model
void Renderer::setLightingMode(std::size_t modelIndex, LightingMode mode)
{
    assert(modelIndex < m_Models.size());
    m_Models[modelIndex].lightingMode = mode;
}

// check whether all data are prepared for model and specific render style
// support some check that is not proper to do in display
void Renderer::checkForModelAttributes()
{
    for (std::size_t i = 0; i < m_Models.size(); ++i)
    {
        if (m_Models[i].style == LightingMaterialTexture)
        {
            if (!m_Models[i].spModel->supplyNormals())
            {
                Utils::Logger::globalLogger().warning("Model " + std::to_string(i) + " set to LightingMaterialTexture render style but normals are not supplied.");
            }
        }
    }
}

// when left mouse button is holding, update view matrix according cursor change
// the cursor coordinate
// O -|------------> X
//    |
//    |
//    |
//    v Y
// 1080 pixels for one circle/360 degrees/2*PI radian
// The rotation logic is just like Blender !
// keep y axis always be vertical in screen pixels, so use y-axis as x rotation axis, not up vector.
void Renderer::updateViewArgsAccordingToCursorPos()
{
    if (getHoldLeftMouseButton(m_pWindow))
    {
        int deltaX = getCursorPosX(m_pWindow) - getLastCursorPosX(m_pWindow);
        int deltaY = getCursorPosY(m_pWindow) - getLastCursorPosY(m_pWindow);
        float xRotationAngle = -2 * glm::pi<float>() / 1080.0f * deltaX;
        float yRotationAngle = 2 * glm::pi<float>() / 1080.0f * deltaY;
        glm::vec3& eyeLocation = getEyeLocation(m_pWindow);
        glm::vec3& objectLocation = getObjectLocation(m_pWindow);
        glm::vec3& upVector = getUpVector(m_pWindow);
        if (deltaX != 0)
        {
            auto dotRes = glm::dot(upVector, glm::vec3(0.0f, 1.0f, 0.0f));
            if (dotRes < 0) // > 90 degrees
            {
                xRotationAngle = -xRotationAngle;
            }
            // the rotation axis of X is y-axis, this is the key point!
            glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), xRotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));
            upVector = rotationMat * glm::vec4(upVector, 1.0f);
            eyeLocation = glm::vec3(rotationMat * glm::vec4(eyeLocation - objectLocation, 1.0f)) + objectLocation;
        }
        if (deltaY != 0)
        {
            // the rotation axis of Y is cross(Eye - Object, Up)
            glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), yRotationAngle, glm::cross(eyeLocation - objectLocation, upVector));
            upVector = rotationMat * glm::vec4(upVector, 1.0f);
            eyeLocation = glm::vec3(rotationMat * glm::vec4(eyeLocation - objectLocation, 1.0f)) + objectLocation;
        }
    }
    getLastCursorPosX(m_pWindow) = getCursorPosX(m_pWindow);
    getLastCursorPosY(m_pWindow) = getCursorPosY(m_pWindow);
}

void Renderer::drawAxises()
{
    if (m_bEnableAxises)
    {
        glUseProgram(m_AxisesShaderProgram);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        m_ModelMatrix = glm::mat4(1.0f);
        m_ViewMatrix = glm::lookAt(getEyeLocation(m_pWindow), getObjectLocation(m_pWindow), getUpVector(m_pWindow));
        m_ModelViewMatrix = m_ViewMatrix * m_ModelMatrix;

        GLuint lenLoc = glGetUniformLocation(m_AxisesShaderProgram, "axisLength");
        GLuint mvLoc = glGetUniformLocation(m_AxisesShaderProgram, "mvMatrix");
        GLuint projLoc = glGetUniformLocation(m_AxisesShaderProgram, "projMatrix");
        glUniform1f(lenLoc, m_AxisLength);
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_ModelViewMatrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(getProjMatrix(m_pWindow)));

        glBindBuffer(GL_ARRAY_BUFFER, m_AxisesVbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_LINES, 0, 6);
    }
}

// display models
void Renderer::display(float currentTime)
{
    // clear background to black during every rendering
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    drawAxises();

    for (std::size_t i = 0; i < m_Models.size(); ++i)
    {
        // render style for different render program
        RenderStyle style = m_Models[i].style;
        GLenum primitiveType = GL_TRIANGLES;
        GLuint  program = 0;
        switch (style)
        {
        case PureColorPoints:
            primitiveType = GL_POINTS;
            program = m_PureColorShaderProgram;
            break;
        case PureColorLines:
            primitiveType = GL_LINES;
            program = m_PureColorShaderProgram;
            break;
        case PureColorLineStrip:
            primitiveType = GL_LINE_STRIP;
            program = m_PureColorShaderProgram;
            break;
        case PureColorTriangles:
            primitiveType = GL_TRIANGLES;
            program = m_PureColorShaderProgram;
            break;
        case VaryingColorPoints:
            primitiveType = GL_POINTS;
            program = m_VaryingColorShaderProgram;
            break;
        case VaryingColorLines:
            primitiveType = GL_LINES;
            program = m_VaryingColorShaderProgram;
            break;
        case VaryingColorLineStrip:
            primitiveType = GL_LINE_STRIP;
            program = m_VaryingColorShaderProgram;
            break;
        case VaryingColorTriangles:
            primitiveType = GL_TRIANGLES;
            program = m_VaryingColorShaderProgram;
            break;
        case SpecificTexture:
            primitiveType = GL_TRIANGLES;
            program = m_TextureShaderProgram;
            break;
        case LightingMaterialTexture:
            primitiveType = GL_TRIANGLES;
            if (m_Models[i].lightingMode == FlatShading || m_Models[i].lightingMode == GouraudShading)
            {
                program = m_GouraudLightingMaterialTextureShderProgram;
            }
            else // Phong shading
            {
                program = m_PhongLightingMaterialTextureShaderProgram;
            }
            break;
        default:
            primitiveType = GL_TRIANGLES;
            program = m_PureColorShaderProgram;
            break;
        }
        glUseProgram(program);

        // backface culling 
        if (m_bEnableCullFace)
        {
            glEnable(GL_CULL_FACE);
            glCullFace(m_FaceCullingMode);
            glFrontFace(m_FrontFace);
        }
        // depth test
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // model matrix
        // rotation
        m_ModelMatrix = glm::mat4(1.0f);
        if (m_Models[i].bRotate)
        {
            m_ModelMatrix = glm::rotate(m_ModelMatrix, currentTime * m_Models[i].rotationRate, m_Models[i].rotationAxis);
        }
        // view matrix
        m_ViewMatrix = glm::lookAt(getEyeLocation(m_pWindow), getObjectLocation(m_pWindow), getUpVector(m_pWindow));

        // model-view matrix
        m_ModelViewMatrix = m_ViewMatrix * m_ModelMatrix;

        GLuint mvLoc = glGetUniformLocation(program, "mvMatrix");
        GLuint projLoc = glGetUniformLocation(program, "projMatrix");
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(m_ModelViewMatrix));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(getProjMatrix(m_pWindow)));

        // input color for pure color shaders
        if (style == PureColorPoints || style == PureColorLineStrip || style == PureColorLines || style == PureColorTriangles)
        {
            GLuint colorLoc = glGetUniformLocation(program, "inputColor");
            glUniform4fv(colorLoc, 1, glm::value_ptr(m_Models[i].color));
        }

        // draw
        if (m_Models[i].spModel->supplyVertices())
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_Models[i].verticesVbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
        }
        if (m_Models[i].spModel->supplyTexCoords())
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_Models[i].texCoordVbo);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);
        }
        if (m_Models[i].spModel->supplyNormals())
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_Models[i].normalVbo);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(2);
        }
        if (m_Models[i].spModel->supplyTangents())
        {
            glBindBuffer(GL_ARRAY_BUFFER, m_Models[i].sTanVbo);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(3);
            glBindBuffer(GL_ARRAY_BUFFER, m_Models[i].tTanVbo);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(4);
        }

        // texture
        if (style == SpecificTexture || style == LightingMaterialTexture)
        {
            glActiveTexture(GL_TEXTURE0); // always use texture unit 0
            glBindTexture(GL_TEXTURE_2D, m_Models[i].texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            if (m_Models[i].doMipmapping)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glGenerateMipmap(GL_TEXTURE_2D); // generate mipmapping
            }
            if (m_Models[i].doAnisotropicFiltering && GLAD_GL_EXT_texture_filter_anisotropic) // check if anisotropic filtering extension is supported ?
            {
                GLfloat anisoSetting = 0.0f;
                glGetFloatv(GL_TEXTURE_MAX_ANISOTROPY_EXT, &anisoSetting);
                glTextureParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisoSetting); // enable anisotropic filtering
            }
        }

        // lighting and material
        if (style == LightingMaterialTexture)
        {
            // do not check normals and material, assume they are supplied.
            
            // flat shading and Gouraud shading
            if (m_Models[i].lightingMode == FlatShading || m_Models[i].lightingMode == GouraudShading)
            {
                GLuint flatLoc = glGetUniformLocation(program, "flatShading");
                glProgramUniform1i(program, flatLoc, (m_Models[i].lightingMode == FlatShading) ? 1 : 0);
            }
            // global ambient
            GLuint globalAmbLoc = glGetUniformLocation(program, "globalAmbient");
            glProgramUniform4fv(program, globalAmbLoc, 1, glm::value_ptr(m_GlobalAmbient));
            // directional lights
            GLuint sizeLoc = glGetUniformLocation(program, "directionalLightsSize");
            glProgramUniform1ui(program, sizeLoc, GLuint(m_DirectionalLights.size()));
            for (std::size_t j = 0; j < m_DirectionalLights.size(); ++j)
            {
                glm::vec3 directionInViewSpace = glm::vec3(glm::transpose(glm::inverse(m_ViewMatrix)) * glm::vec4(m_DirectionalLights[j].getDirection(), 1.0f));
                std::string str = "directionalLights[" + std::to_string(j) + "]";
                GLuint ambLoc = glGetUniformLocation(program, (str + ".ambient").c_str());
                GLuint diffLoc = glGetUniformLocation(program, (str + ".diffuse").c_str());
                GLuint specLoc = glGetUniformLocation(program, (str + ".specular").c_str());
                GLuint dirLoc = glGetUniformLocation(program, (str + ".direction").c_str());
                glProgramUniform4fv(program, ambLoc, 1, glm::value_ptr(m_DirectionalLights[j].getAmbient()));
                glProgramUniform4fv(program, diffLoc, 1, glm::value_ptr(m_DirectionalLights[j].getDiffuse()));
                glProgramUniform4fv(program, specLoc, 1, glm::value_ptr(m_DirectionalLights[j].getSpecular()));
                glProgramUniform3fv(program, dirLoc, 1, glm::value_ptr(directionInViewSpace));
            }
            // point lights
            sizeLoc = glGetUniformLocation(program, "pointLightsSize");
            glProgramUniform1ui(program, sizeLoc, GLuint(m_PointLights.size()));
            for (std::size_t j = 0; j < m_PointLights.size(); ++j)
            {
                glm::vec3 locationInViewSpace = glm::vec3(m_ViewMatrix * glm::vec4(m_PointLights[j].getLocation(), 1.0f));
                std::string str = "pointLights[" + std::to_string(j) + "]";
                GLuint ambLoc = glGetUniformLocation(program, (str + ".ambient").c_str());
                GLuint diffLoc = glGetUniformLocation(program, (str + ".diffuse").c_str());
                GLuint specLoc = glGetUniformLocation(program, (str + ".specular").c_str());
                GLuint locLoc = glGetUniformLocation(program, (str + ".location").c_str());
                GLuint constantLoc = glGetUniformLocation(program, (str + ".constant").c_str());
                GLuint linearLoc = glGetUniformLocation(program, (str + ".linear").c_str());
                GLuint quadraticLoc = glGetUniformLocation(program, (str + ".quadratic").c_str());
                glProgramUniform4fv(program, ambLoc, 1, glm::value_ptr(m_PointLights[j].getAmbient()));
                glProgramUniform4fv(program, diffLoc, 1, glm::value_ptr(m_PointLights[j].getDiffuse()));
                glProgramUniform4fv(program, specLoc, 1, glm::value_ptr(m_PointLights[j].getSpecular()));
                glProgramUniform3fv(program, locLoc, 1, glm::value_ptr(locationInViewSpace));
                glProgramUniform1f(program, constantLoc, m_PointLights[j].getConstant());
                glProgramUniform1f(program, linearLoc, m_PointLights[j].getLinear());
                glProgramUniform1f(program, quadraticLoc, m_PointLights[j].getQuadratic());
            }
            // spot lights
            sizeLoc = glGetUniformLocation(program, "spotLightsSize");
            glProgramUniform1ui(program, sizeLoc, GLuint(m_SpotLights.size()));
            for (std::size_t j = 0; j < m_SpotLights.size(); ++j)
            {
                glm::vec3 locationInViewSpace = glm::vec3(m_ViewMatrix * glm::vec4(m_SpotLights[j].getLocation(), 1.0f));
                glm::vec3 directionInViewSpace = glm::vec3(glm::transpose(glm::inverse(m_ViewMatrix)) * glm::vec4(m_SpotLights[j].getDirection(), 1.0f));
                std::string str = "spotLights[" + std::to_string(j) + "]";
                GLuint ambLoc = glGetUniformLocation(program, (str + ".ambient").c_str());
                GLuint diffLoc = glGetUniformLocation(program, (str + ".diffuse").c_str());
                GLuint specLoc = glGetUniformLocation(program, (str + ".specular").c_str());
                GLuint locLoc = glGetUniformLocation(program, (str + ".location").c_str());
                GLuint dirLoc = glGetUniformLocation(program, (str + ".direction").c_str());
                GLuint cutoffLoc = glGetUniformLocation(program, (str + ".cutOffAngle").c_str());
                GLuint exponentLoc = glGetUniformLocation(program, (str + ".strengthFactorExponent").c_str());
                glProgramUniform4fv(program, ambLoc, 1, glm::value_ptr(m_SpotLights[j].getAmbient()));
                glProgramUniform4fv(program, diffLoc, 1, glm::value_ptr(m_SpotLights[j].getDiffuse()));
                glProgramUniform4fv(program, specLoc, 1, glm::value_ptr(m_SpotLights[j].getSpecular()));
                glProgramUniform3fv(program, locLoc, 1, glm::value_ptr(locationInViewSpace));
                glProgramUniform3fv(program, dirLoc, 1, glm::value_ptr(directionInViewSpace));
                glProgramUniform1f(program, cutoffLoc, m_SpotLights[j].getCutOffAngle());
                glProgramUniform1f(program, exponentLoc, m_SpotLights[j].getStrengthFactorExponent());
            }
            // material and texture weight
            GLuint materialWeightLoc = glGetUniformLocation(program, "materialWeight");
            glProgramUniform1f(program, materialWeightLoc, m_Models[i].materialWeight);
            GLuint textureWeightLoc = glGetUniformLocation(program, "textureWeight");
            glProgramUniform1f(program, textureWeightLoc, m_Models[i].textureWeight);
            // material
            if (m_Models[i].spMaterial)
            {
                GLuint mAmbLoc = glGetUniformLocation(program, "material.ambient");
                GLuint mDiffLoc = glGetUniformLocation(program, "material.diffuse");
                GLuint mSpecLoc = glGetUniformLocation(program, "material.specular");
                GLuint mShiLoc = glGetUniformLocation(program, "material.shininess");
                glProgramUniform4fv(program, mAmbLoc, 1, glm::value_ptr(m_Models[i].spMaterial->getAmbient()));
                glProgramUniform4fv(program, mDiffLoc, 1, glm::value_ptr(m_Models[i].spMaterial->getDiffuse()));
                glProgramUniform4fv(program, mSpecLoc, 1, glm::value_ptr(m_Models[i].spMaterial->getSpecular()));
                glProgramUniform1f(program, mShiLoc, m_Models[i].spMaterial->getShininess());
            }

            // build the inverse transpose of model-view matrix to transform vertex normal
            glm::mat4 inverseTransposeMvMatrix = glm::transpose(glm::inverse(m_ModelViewMatrix));
            GLuint nLoc = glGetUniformLocation(program, "normMatrix");
            glUniformMatrix4fv(nLoc, 1, GL_FALSE, glm::value_ptr(inverseTransposeMvMatrix));
        }

        if (m_Models[i].spModel->supplyIndices())
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Models[i].indicesVbo);
            glDrawElements(primitiveType, m_Models[i].verticesCount, GL_UNSIGNED_INT, 0);
        }
        else
        {
            glDrawArrays(primitiveType, 0, m_Models[i].verticesCount);
        }
    }
}

} // namespace Utils