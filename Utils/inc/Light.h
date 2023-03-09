#pragma once
#include <glm/glm.hpp>

namespace Utils
{

// global ambient light
class GlobalAmbientLight
{
private:
    glm::vec4 m_Ambient;
public:
    GlobalAmbientLight(glm::vec4 ambient);
    const glm::vec4& getAmbient() const;
};

// directional light, like sun light
class DirectionalLight
{
private:
    glm::vec4 m_Ambient;
    glm::vec4 m_Diffuse;
    glm::vec4 m_Specular;
    glm::vec3 m_Direction;
public:
    DirectionalLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 direction);
    const glm::vec4& getAmbient() const;
    const glm::vec4& getDiffuse() const;
    const glm::vec4& getSpecular() const;
    const glm::vec3& getDirection() const;
};

// point light, like a lamp
class PointLight
{
private:
    glm::vec4 m_Ambient;
    glm::vec4 m_Diffuse;
    glm::vec4 m_Specular;
    glm::vec3 m_Location;
    // to calculate attenuation: 1 / (c + l*d + q*d*d)
    float m_Constant;
    float m_Linear;
    float m_Quadratic;
public:
    PointLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, float constant, float linear, float quadratic);
    const glm::vec4& getAmbient() const;
    const glm::vec4& getDiffuse() const;
    const glm::vec4& getSpecular() const;
    const glm::vec3& getLocation() const;
    float getConstant() const;
    float getLinear() const;
    float getQuadratic() const;
};

// spot light
class SpotLight
{
private:
    glm::vec4 m_Ambient;
    glm::vec4 m_Diffuse;
    glm::vec4 m_Specular;
    glm::vec3 m_Location;
    glm::vec3 m_Direction;
    float m_CutOffAngle;                // cut off angle
    float m_StrengthFactorExponent;     // the exponent of strength factor, the strength factor will be (cos(theta))^m_Exponent
    // float m_Attenuation;                // attenuation factor
public:
    SpotLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, glm::vec3 direction, float cutoff, float exponent);
    const glm::vec4& getAmbient() const;
    const glm::vec4& getDiffuse() const;
    const glm::vec4& getSpecular() const;
    const glm::vec3& getLocation() const;
    const glm::vec3& getDirection() const;
    float getCutOffAngle() const;
    float getStrengthFactorExponent() const;
};

} // namespace Utils
