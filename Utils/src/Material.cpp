#include <Material.h>

namespace Utils
{

Material::Material(glm::vec4 amibent, glm::vec4 diffuse, glm::vec4 specular, float shininess)
    : m_amibent(amibent)
    , m_diffuse(diffuse)
    , m_specular(specular)
    , m_shininess(shininess)
{
}

void Material::setAmbient(glm::vec4 ambient)
{
    m_amibent = ambient;
}

void Material::setDiffuse(glm::vec4 diffuse)
{
    m_diffuse = diffuse;
}

void Material::setSpecular(glm::vec4 specular)
{
    m_specular = specular;
}

void Material::setShininess(float shininess)
{
    m_shininess = shininess;
}

const glm::vec4& Material::getAmbient()
{
    return m_amibent;
}

const glm::vec4& Material::getDiffuse()
{
    return m_diffuse;
}

const glm::vec4& Material::getSpecular()
{
    return m_specular;
}

float Material::getShininess()
{
    return m_shininess;
}

} // namespace Utils