#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <unordered_map>
#include "Material.h"
#include "Model.h"
#include "Logger.h"
#include "Light.h"
#include "Shader.h"

namespace Utils
{

class Renderer
{
public:
    enum RenderStyle
    {
        PureColorPoints,            // render to pure color isolated points
        PureColorLines,             // render to pure color lines
        PureColorLineStrip,         // render to pure color line strip
        PureColorTriangles,         // render to pure color triangles
        VaryingColorPoints,         // varying color points according to coordinate
        VaryingColorLines,          // varying color lines according to coordinate
        VaryingColorLineStrip,      // varying color line strips according to coordinate
        VaryingColorTriangles,      // varying color triangles according to coordinate
        SpecificTexture,            // render to specific texture
        LightingMaterialTexture,    // support lighting, mix texture and material with specific weight
        PhongShadingWithShadow,     // Phong shading, but support shadows, only models with this mode will create shadow and affected by shadow
        EnvironmentMap              // draw environment map to model, sky box as environment
    };
    // lighting rendering mode only for LightingMaterialTexture style
    enum LightingMode
    {
        FlatShading,            // the old shading mode, do not interpolate, aka patch shading
        GouraudShading,         // one way of smooth shading
        PhongShading            // another way of smooth shading
    };
    // Percentage Closer Filtering/PCF mode to generate soft shadow, only for PhongShadingWithShadow mode.
    enum PCFMode
    {
        NoPCF = 0,          // disable PCF
        Sample64,           // sampling for near 64 texels
        Sample4Dithered     // sampling for near dithered 4 texels
    };
private:
    struct ModelAttributes
    {
        // model, render style, vertices count
        std::shared_ptr<Model> spModel = nullptr;
        RenderStyle style = VaryingColorLineStrip;
        GLsizei verticesCount = 0;
        // color, default to pure white, only for pure color styles
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        // texture
        GLuint texture = 0; // texture id
        bool doMipmapping = true;
        bool doAnisotropicFiltering = true;
        float textureWeight = 0.0;
        // material, for LightingMaterialTexture style
        std::unique_ptr<Material> spMaterial = nullptr;
        float materialWeight = 0.0;
        // lighting mode, for LightingMaterialTexture style
        LightingMode lightingMode = FlatShading;
        // rotation attributes
        bool bRotate = false;
        glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        float rotationRate = 1.0; // means 1 second for 1 radian
        // vao, one vao per model
        GLuint vao = 0;
        // vbos
        GLuint indicesVbo = 0;
        GLuint verticesVbo = 0;
        GLuint texCoordVbo = 0;
        GLuint normalVbo = 0;
        GLuint sTanVbo = 0;
        GLuint tTanVbo = 0;
    };
    // attributes that every window needs one copy
    // workaround for variables that need to be visited in call back function, they must be static, so save them in static hashtable for every window.
    struct WindowAttributes
    {
        // view matrix parameters
        glm::vec3 m_EyeLocation = glm::vec3(0.0f, 10.0f, 10.0f);
        glm::vec3 m_ObjectLocation = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_UpVector = glm::vec3(0.0f, 1.0f, -1.0f);
        // projection matrix
        glm::mat4 m_ProjMatrix = glm::mat4(1.0f);
        // mouse input variables
        bool m_bHoldLeftMouseButton = false;
        int m_CursorPosX = 0;
        int m_CursorPosY = 0;
        int m_LastCursorPosX = 0;
        int m_LastCursorPosY = 0;
    };
    inline static std::unordered_map<GLFWwindow*, WindowAttributes> s_WindowAttrs;
    // get window attributes of this window, all inline
    static glm::vec3& getEyeLocation(GLFWwindow* w) { return s_WindowAttrs[w].m_EyeLocation; }
    static glm::vec3& getObjectLocation(GLFWwindow* w) { return s_WindowAttrs[w].m_ObjectLocation; }
    static glm::vec3& getUpVector(GLFWwindow* w) { return s_WindowAttrs[w].m_UpVector; }
    static glm::mat4& getProjMatrix(GLFWwindow* w) { return s_WindowAttrs[w].m_ProjMatrix; }
    static bool& getHoldLeftMouseButton(GLFWwindow* w) { return s_WindowAttrs[w].m_bHoldLeftMouseButton; }
    static int& getCursorPosX(GLFWwindow* w) { return s_WindowAttrs[w].m_CursorPosX; }
    static int& getCursorPosY(GLFWwindow* w) { return s_WindowAttrs[w].m_CursorPosY; }
    static int& getLastCursorPosX(GLFWwindow* w) { return s_WindowAttrs[w].m_LastCursorPosX; }
    static int& getLastCursorPosY(GLFWwindow* w) { return s_WindowAttrs[w].m_LastCursorPosY; }
private:
    // some max lihgts number constants, must be same as the number in the shader !
    static constexpr std::size_t MAX_POINT_LIGHT_SIZE = 5;
    static constexpr std::size_t MAX_DIRECTIONAL_LIGHT_SIZE = 5;
    static constexpr std::size_t MAX_SPOT_LIGHT_SIZE = 5;
    static constexpr std::size_t MAX_SHADOW_SIZE = MAX_POINT_LIGHT_SIZE + MAX_DIRECTIONAL_LIGHT_SIZE + MAX_SPOT_LIGHT_SIZE; // 15
private:
    // window
    GLFWwindow* m_pWindow = nullptr;
    // lighting
    bool m_bEnableLighting = false;
    // face culling
    bool m_bEnableCullFace = true;
    GLenum m_FaceCullingMode = GL_BACK;
    GLenum m_FrontFace = GL_CCW;
    // models
    std::vector<ModelAttributes> m_Models;
    // matrices
    glm::mat4 m_ModelMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ModelViewMatrix;
    // predefined rendering programs
    Shader m_AxisesShader;
    Shader m_PureColorShader;
    Shader m_VaryingColorShader;
    Shader m_TextureShader;
    Shader m_GouraudMaterialTextureShader;
    Shader m_PhongMaterialTextureShader;
    Shader m_SimpleShadowDepthShader;   // generate depth shadow texture for every light
    Shader m_ShadowShader;              // draw shadows use shadow texture
    Shader m_ShadowDebugShader1;        // just show the specific shadow texture.
    Shader m_ShadowDebugShader2;        // show simplified shadow result for specific light.
    Shader m_SkyBoxShader;              // render sky box
    Shader m_EnvironmentMapShader;      // render environment map (use texture of sky box) as texture
    // xyz axis
    GLuint m_AxisesVao;
    GLuint m_AxisesVbo;
    bool m_bEnableAxises = true;
    float m_AxisLength = 100.0f;
    // light sources
    glm::vec4 m_GlobalAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    std::vector<DirectionalLight> m_DirectionalLights;
    std::vector<PointLight> m_PointLights;
    std::vector<SpotLight> m_SpotLights;
    // shadow related variables: shadow textuers, shadow buffers, etc
    std::vector<GLuint> m_ShadowTextures;
    std::vector<GLuint> m_ShadowBuffers;
    std::vector<glm::mat4> m_ShadowVPs;
    GLuint m_FirstShadowTextureUnit = GL_TEXTURE10; // shadow texture begin from texture unit 10
    glm::mat4 m_BMatrix;
    PCFMode m_PCFMode = NoPCF;
    float m_PCFFactor = 2.5f;
    // sky box
    GLuint m_SkyBoxVao;
    GLuint m_SkyBoxVbo;
    bool m_bEanbleSkyBox = false;
    GLuint m_SkyBoxTexture = 0;
    GLsizei m_SkyBoxVerticesCount = 0;
public:
    Renderer(const char* windowTitle, int width = 1920, int height = 1080, float axisLength = 100.0f);
    ~Renderer();

    // run the render loop
    void run();
    
    // add model to render, return it's index
    std::size_t addModel(std::shared_ptr<Model> spModel, RenderStyle renderStyle);

    // add lighting to render, affect those objects with lighting modes
    void setGlobalAmbientLight(glm::vec4 ambient);
    void addDirectionalLight(const DirectionalLight& light);
    void addDirectionalLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 direction);
    void addPointLight(const PointLight& light);
    void addPointLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, float constant, float linear, float quadratic);
    void addSpotLight(const SpotLight& light);
    void addSpotLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, glm::vec3 direction, float cutoff, float exponent);

    // set face culling attributes, defualt to true/GL_BACK/GL_CCW
    void setFaceCullingAttribute(bool enable, GLenum mode, GLenum front); 
    
    // set PCF(Percentage Closer Filtering) mode, for soft shadow, default to NoPCF, only affect models with PhongShadingWithShadow style
    // set pcf factor to adjust the diffusion range of soft shadow, a typical value is 2.5f
    void setPCFMode(PCFMode mode, float pcfFactor = 2.5f);

    // enable sky box, set texture to sky box
    void enableSkyBox(const char* rightImage, const char* leftImage,
                   const char* topImage, const char* bottomImage,
                   const char* frontImage, const char* backImage);

    // set model attributes
    // set rotatoin attributes, default to false
    void setModelRotation(std::size_t modelIndex, glm::vec3 rotationAxis, float rotationRate);
    // set render style
    void setRenderStyle(std::size_t modelIndex, RenderStyle renderStyle);
    // set render color, just for pure color style, default to pure white
    void setColor(std::size_t modelIndex, glm::vec4 color);
    // set texture for model, for SpecificTexture/LightingMaterialTexture style
    void setTexture(std::size_t modelIndex, const char* textureImagePath, float weight = 1.0, bool doMipmapping = true, bool doAnisotropicFiltering = true);
    void setTexture(std::size_t modelIndex, GLuint textureId, float weight = 1.0, bool doMipmapping = true, bool doAnisotropicFiltering = true);
    // set material for model, for LightingMaterialTexture style
    void setMaterial(std::size_t modelIndex, const Material& material, float weight = 1.0);
    // set lighting mode of model, for LightingMaterialTexture style, default to FlatShading
    void setLightingMode(std::size_t modelIndex, LightingMode mode);
private:
    void checkForModelAttributes();
    void updateViewArgsAccordingToCursorPos();
    void drawAxises();
    void drawShadowTextures(float currentTime);
    void drawSkyBox();
    void display(float currentTime);
    // debug functions
    void debugShowShadowTexture(std::size_t shadowIndex);
    void debugShowSimplifiedShadowResult(std::size_t shadowIndex, float currentTime);
};

} // namespace Utils