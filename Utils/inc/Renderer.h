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
        LightingMaterialTexture     // support lighting, mix texture and material with specific weight
    };
    enum LightingMode
    {
        FlatShading,            // the old shading mode, do not interpolate, aka patch shading
        GouraudShading,         // one way of smooth shading
        PhongShading            // another way of smooth shading
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
        // material, for LightingMaterial style
        std::unique_ptr<Material> spMaterial = nullptr;
        float materialWeight = 0.0;
        // lighting mode, for LightingMaterial/LightingTexture style
        LightingMode lightingMode = FlatShading;
        // rotation attributes
        bool bRotate = false;
        glm::vec3 rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);
        float rotationRate = 1.0; // means 1 second for 1 radian
        // OpenGL vbos
        GLuint indicesVbo;
        GLuint verticesVbo;
        GLuint texCoordVbo;
        GLuint normalVbo;
        GLuint sTanVbo;
        GLuint tTanVbo;
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
    static constexpr std::size_t MAX_POINT_LIGHT_SIZE = 10;
    static constexpr std::size_t MAX_DIRECTIONAL_LIGHT_SIZE = 10;
    static constexpr std::size_t MAX_SPOT_LIGHT_SIZE = 10;
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
    // vao 
    std::vector<GLuint> m_VaoVec;
    // predefined rendering programs
    GLuint m_AxisesShaderProgram;
    GLuint m_PureColorShaderProgram;
    GLuint m_VaryingColorShaderProgram;
    GLuint m_TextureShaderProgram;
    GLuint m_GouraudLightingMaterialTextureShderProgram;
    GLuint m_PhongLightingMaterialTextureShaderProgram;
    // xyz axis
    GLuint m_AxisesVbo;
    bool m_bEnableAxises = true;
    float m_AxisLength = 100.0f;
    // light sources
    glm::vec4 m_GlobalAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    std::vector<DirectionalLight> m_DirectionalLights;
    std::vector<PointLight> m_PointLights;
    std::vector<SpotLight> m_SpotLights;
public:
    Renderer(const char* windowTitle, int width = 1920, int height = 1080, float axisLength = 100.0f);
    ~Renderer();

    // run the render loop
    void run();
    
    // add model to render, return it's index
    std::size_t addModel(std::shared_ptr<Model> spModel, RenderStyle renderStyle);

    // add lighting to render, affect those objects with lighting modes (LightingMaterial/LightingTexture)
    void setGlobalAmbientLight(glm::vec4 ambient);
    void addDirectionalLight(const DirectionalLight& light);
    void addDirectionalLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 direction);
    void addPointLight(const PointLight& light);
    void addPointLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, float constant, float linear, float quadratic);
    void addSpotLight(const SpotLight& light);
    void addSpotLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, glm::vec3 direction, float cutoff, float exponent);

    // set face culling attributes, defualt to true/GL_BACK/GL_CCW
    void setFaceCullingAttribute(bool enable, GLenum mode, GLenum front); 

    // set model attributes
    // set rotatoin attributes, default to false
    void setModelRotation(std::size_t modelIndex, glm::vec3 rotationAxis, float rotationRate);
    // set render style
    void setRenderStyle(std::size_t modelIndex, RenderStyle renderStyle);
    // set render color, just for pure color style, default to pure white
    void setColor(std::size_t modelIndex, glm::vec4 color);
    // set texture for model, just for SpecificTexture style
    void setTexture(std::size_t modelIndex, const char* textureImagePath, float weight = 1.0, bool doMipmapping = true, bool doAnisotropicFiltering = true);
    void setTexture(std::size_t modelIndex, GLuint textureId, float weight = 1.0, bool doMipmapping = true, bool doAnisotropicFiltering = true);
    // set material for model, for LightingMaterial style
    void setMaterial(std::size_t modelIndex, const Material& material, float weight = 1.0);
    // set lighting mode of model, default to FlatShading
    void setLightingMode(std::size_t modelIndex, LightingMode mode);
private:
    void checkForModelAttributes();
    void updateViewArgsAccordingToCursorPos();
    void drawAxises();
    void display(float currentTime);
};

} // namespace Utils