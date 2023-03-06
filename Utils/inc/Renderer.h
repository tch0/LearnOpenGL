#pragma once
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <string>
#include <functional>
#include <memory>
#include "Material.h"
#include "Model.h"

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
        LightingMaterial,           // support lighting, render to specific material
        LightingTexture             // combine lighting with texture
    };
    struct ModelAttributes
    {
        std::shared_ptr<Model> spModel = nullptr;
        RenderStyle style = VaryingColorLineStrip;
        GLsizei verticesCount = 0;
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f); // default to pure white, only for pure color styles
        GLuint texture = 0; // texture id, only for SpecificTexture style
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
    // view matrix parameters
    inline static glm::vec3 m_EyeLocation = glm::vec3(0.0f, 10.0f, 10.0f);
    inline static glm::vec3 m_ObjectLocation = glm::vec3(0.0f, 0.0f, 0.0f);
    inline static glm::vec3 m_UpVector = glm::vec3(0.0f, 1.0f, -1.0f);
    // matrices
    glm::mat4 m_ModelMatrix;
    glm::mat4 m_ViewMatrix;
    glm::mat4 m_ModelViewMatrix;
    inline static glm::mat4 m_ProjMatrix;
    // vao 
    std::vector<GLuint> m_VaoVec;
    // vbos
    std::vector<GLuint> m_VboVec;
    // predefined rendering programs
    GLuint m_AxisesShaderProgram;
    GLuint m_PureColorShaderProgram;
    GLuint m_VaryingColorShaderProgram;
    GLuint m_TextureShaderProgram;
    GLuint m_LightingShderProgram;
    // mouse input vaiables
    inline static bool m_bHoldLeftMouseButton = false;
    inline static int m_CursorPosX = 0;
    inline static int m_CursorPosY = 0;
    inline static int m_LastCursorPosX = 0;
    inline static int m_LastCursorPosY = 0;
    // xyz axis
    GLuint m_AxisesVbo;
    bool m_bEnableAxises = true;
    float m_AxisLength = 100.0f;
public:
    Renderer(const char* windowTitle, int width = 1920, int height = 1080, float axisLength = 100.0f);
    ~Renderer();
    void run(); // run the render loop
    
    // add model to render, return it's index
    int addModel(std::shared_ptr<Model> spModel, RenderStyle renderStyle);

    // set face culling attributes, defualt to true/GL_BACK/GL_CCW
    void setFaceCullingAttribute(bool enable, GLenum mode, GLenum front); 

    // set model attributes
    // set rotatoin attributes, default to false
    void setModelRotation(int modelIndex, glm::vec3 rotationAxis, float rotationRate);
    // set render style
    void setRenderStyle(int modelIndex, RenderStyle RenderStyle);
    // set render color, just for pure color style, default to pure white
    void setColor(int modelIndex, glm::vec4 color);
    // set texture for model, just for SpecificTexture style
    void setTexture(int modelIndex, const char* textureImagePath);
    void setTexture(int modelIndex, GLuint textureId);
    // lighting: todo
private:
    void updateViewArgsAccordingToCursorPos();
    void drawAxises();
    void display(float currentTime);
};

} // namespace Utils