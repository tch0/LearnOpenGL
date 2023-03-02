#include <Sphere.h>
#include <glm/ext.hpp>
#include <cmath>

namespace Utils
{

Sphere::Sphere(int prec)
{
    init(prec);
}

void Sphere::init(int prec)
{
    m_numVertices = (prec + 1) * (prec + 1);
    m_numIndices = prec * prec * 6;
    m_indices.resize(m_numIndices);
    m_vertices.resize(m_numVertices);
    m_texCoords.resize(m_numVertices);
    m_normals.resize(m_numVertices);

    // calculate triangle vertices
    for (int i = 0; i <= prec; i++)
    {
        for (int j = 0; j <= prec; j++)
        {
            float y = float(cos(glm::radians(180.f - i * 180.0f / prec)));
            float x = -float(cos(glm::radians(j*360.f / prec))) * float(abs(cos(asin(y))));
            float z = float(sin(glm::radians(j*360.f / prec))) * float(abs(cos(asin(y))));
            m_vertices[i*(prec+1) + j] = glm::vec3(x, y, z);
            m_texCoords[i*(prec+1) + j] = glm::vec2(float(j) / prec, float(i) / prec);
            m_normals[i*(prec+1) + j] = glm::vec3(x, y, z);
        }
    }

    // calculate triangle indices
    for (int i = 0; i < prec; i++)
    {
        for (int j = 0; j < prec; j++)
        {
            m_indices[6 * (i*prec + j) + 0] = i*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 1] = i*(prec + 1) + j + 1;
            m_indices[6 * (i*prec + j) + 2] = (i + 1)*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 3] = i*(prec + 1) + j + 1;
            m_indices[6 * (i*prec + j) + 4] = (i + 1)*(prec + 1) + j + 1;
            m_indices[6 * (i*prec + j) + 5] = (i + 1)*(prec + 1) + j;
        }
    }
}

std::vector<float> Sphere::getVerticesArray()
{
    std::vector<float> verticesVec;
    verticesVec.reserve(m_numIndices * 3);
    for (int i = 0; i < m_numIndices; ++i)
    {
        verticesVec.push_back(m_vertices[m_indices[i]].x);
        verticesVec.push_back(m_vertices[m_indices[i]].y);
        verticesVec.push_back(m_vertices[m_indices[i]].z);
    }
    return verticesVec;
}

std::vector<float> Sphere::getTexCoordsArray()
{
    std::vector<float> texCoordsVec;
    texCoordsVec.reserve(m_numIndices * 2);
    for (int i = 0; i < m_numIndices; ++i)
    {
        texCoordsVec.push_back(m_texCoords[m_indices[i]].s);
        texCoordsVec.push_back(m_texCoords[m_indices[i]].t);
    }
    return texCoordsVec;
}

std::vector<float> Sphere::getNormalsArray()
{
    std::vector<float> normalsVec;
    normalsVec.reserve(m_numIndices * 3);
    for (int i = 0; i < m_numIndices; ++i)
    {
        normalsVec.push_back(m_normals[m_indices[i]].x);
        normalsVec.push_back(m_normals[m_indices[i]].y);
        normalsVec.push_back(m_normals[m_indices[i]].z);
    }
    return normalsVec;
}

std::vector<int> Sphere::getIndices()
{
    return m_indices;
}

std::vector<glm::vec3> Sphere::getVertices()
{
    return m_vertices;
}

std::vector<glm::vec2> Sphere::getTexCoords()
{
    return m_texCoords;
}

std::vector<glm::vec3> Sphere::getNormals()
{
    return m_normals;
}


} // namespace Utils