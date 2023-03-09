#pragma once
#include <glm/glm.hpp>

namespace Utils
{

class Material
{
private:
    // color in RGBA
    glm::vec4 m_amibent;
    glm::vec4 m_diffuse;
    glm::vec4 m_specular;
    float m_shininess; // to establish specular highlight
public:
    Material(glm::vec4 amibent, glm::vec4 diffuse, glm::vec4 specular, float shininess);
    void setAmbient(glm::vec4 ambient);
    void setDiffuse(glm::vec4 diffuse);
    void setSpecular(glm::vec4 specular);
    void setShininess(float shininess);
    const glm::vec4& getAmbient();
    const glm::vec4& getDiffuse();
    const glm::vec4& getSpecular();
    float getShininess();
};

// need C++17 to create inline variable
// some predefined materials
inline Material goldMaterial{ glm::vec4(0.24725f, 0.1995f, 0.0745f, 1.0f),
                              glm::vec4(0.75164f, 0.60648f, 0.22648f, 1.0f),
                              glm::vec4(0.62828f, 0.5558f, 0.36607f, 1.0f),
                              51.2f};

inline Material silverMaterial{ glm::vec4(0.19225f, 0.19225f, 0.19225f, 1.0f),
                                glm::vec4(0.50754f, 0.50754f, 0.50754f, 1.0f),
                                glm::vec4(0.50827f, 0.50827f, 0.50827f, 1.0f),
                                51.2f};

inline Material bronzeMaterial{ glm::vec4(0.2125f, 0.1275f, 0.0540f, 1.0f),
                                glm::vec4(0.7140f, 0.4284f, 0.1814f, 1.0f),
                                glm::vec4(0.3936f, 0.2719f, 0.1667f, 1.0f),
                                25.6f};

inline Material jadeMateial{ glm::vec4(0.135f, 0.2225f, 0.1575f, 0.95f),
                             glm::vec4(0.54f, 0.89f, 0.63f, 0.95f),
                             glm::vec4(0.3162f, 0.3162f, 0.3162f, 0.95f),
                             12.8f};

inline Material pearlMateiral{ glm::vec4(0.25f, 0.20725f, 0.20725f, 0.922f),
                               glm::vec4(1.00f, 0.829f, 0.829f, 0.922f),
                               glm::vec4(0.2966f, 0.2966f, 0.2966f, 0.922f),
                               51.2f};

} // namespace Utils