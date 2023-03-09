#include <Light.h>

namespace Utils
{

// ================================ global ambient ================================
GlobalAmbientLight::GlobalAmbientLight(glm::vec4 ambient)
    : m_Ambient(ambient)
{
}
const glm::vec4& GlobalAmbientLight::getAmbient() const
{
    return m_Ambient;
}

// ================================ directional light ================================
DirectionalLight::DirectionalLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 direction)
    : m_Ambient(ambient)
    , m_Diffuse(diffuse)
    , m_Specular(specular)
    , m_Direction(direction)
{
}
const glm::vec4& DirectionalLight::getAmbient() const
{
    return m_Ambient;
}
const glm::vec4& DirectionalLight::getDiffuse() const
{
    return m_Diffuse;
}
const glm::vec4& DirectionalLight::getSpecular() const
{
    return m_Specular;
}
const glm::vec3& DirectionalLight::getDirection() const
{
    return m_Direction;
}

// ================================= point light ======================================
PointLight::PointLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, float constant, float linear, float quadratic)
    : m_Ambient(ambient)
    , m_Diffuse(diffuse)
    , m_Specular(specular)
    , m_Location(location)
    , m_Constant(constant)
    , m_Linear(linear)
    , m_Quadratic(quadratic)
{
}
const glm::vec4& PointLight::getAmbient() const
{
    return m_Ambient;
}
const glm::vec4& PointLight::getDiffuse() const
{
    return m_Diffuse;
}
const glm::vec4& PointLight::getSpecular() const
{
    return m_Specular;
}
const glm::vec3& PointLight::getLocation() const
{
    return m_Location;
}
float PointLight::getConstant() const
{
    return m_Constant;
}
float PointLight::getLinear() const
{
    return m_Linear;
}
float PointLight::getQuadratic() const
{
    return m_Quadratic;
}

// ================================= spot light ======================================
SpotLight::SpotLight(glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 location, glm::vec3 direction, float cutoff, float exponent)
    : m_Ambient(ambient)
    , m_Diffuse(diffuse)
    , m_Specular(specular)
    , m_Location(location)
    , m_Direction(direction)
    , m_CutOffAngle(cutoff)
    , m_StrengthFactorExponent(exponent)
{
}
const glm::vec4& SpotLight::getAmbient() const
{
    return m_Ambient;
}
const glm::vec4& SpotLight::getDiffuse() const
{
    return m_Diffuse;
}
const glm::vec4& SpotLight::getSpecular() const
{
    return m_Specular;
}
const glm::vec3& SpotLight::getLocation() const
{
    return m_Location;
}
const glm::vec3& SpotLight::getDirection() const
{
    return m_Direction;
}
float SpotLight::getCutOffAngle() const
{
    return m_CutOffAngle;
}
float SpotLight::getStrengthFactorExponent() const
{
    return m_StrengthFactorExponent;
}

} // namespace Utils