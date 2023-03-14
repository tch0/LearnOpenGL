#include <Plane.h>

namespace Utils
{

// orthogonal to y axis
Plane::Plane(float y, float width, int prec)
{
    int numVertices = (prec + 1) * (prec + 1);
    int numIndices = prec * prec * 6;
    m_indices.resize(numIndices);
    m_vertices.resize(numVertices);
    m_texCoords.resize(numVertices);
    m_normals.resize(numVertices);
    m_sTangents.resize(numVertices);
    m_tTangents.resize(numVertices);

    for (int i = 0; i <= prec; i++)
    {
        float x = -width / 2.0f;
        float z = -width / 2.0f;
        float texX = 0.0f;
        float texZ = 0.0f;
        for (int j = 0; j <= prec; j++)
        {
            m_vertices[i * (prec + 1) + j] = glm::vec3(x + width/prec*j, y, z + width/prec*i);
            m_texCoords[i * (prec + 1) + j] = glm::vec2(texX + 1.0/prec*j, texZ + 1.0/prec*i);
            m_normals[i * (prec + 1) + j] = glm::vec3(0.0f, 1.0f, 0.0f);
            m_sTangents[i * (prec + 1) + j] = glm::vec3(1.0f, 0.0f, 0.0f);
            m_sTangents[i * (prec + 1) + j] = glm::vec3(0.0f, 0.0f, 1.0f);
        }
    }
    for (int i = 0; i < prec; i++)
    {
        for (int j = 0; j < prec; j++)
        {
            m_indices[6 * (i*prec + j) + 0] = (i)*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 1] = (i+1)*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 2] = (i+1)*(prec + 1) + j + 1;
            m_indices[6 * (i*prec + j) + 3] = (i)*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 4] = (i+1)*(prec + 1) + j + 1;
            m_indices[6 * (i*prec + j) + 5] = (i)*(prec + 1) + j + 1;
        }
    }
}

// match with glDrawArrays
bool Plane::supplyVertices()
{
    return true;
}

bool Plane::supplyTexCoords()
{
    return true;
}

bool Plane::supplyNormals()
{
    return true;
}

bool Plane::supplyTangents()
{
    return true;
}

std::vector<float> Plane::getVerticesArray()
{
    std::vector<float> verticesVec;
    verticesVec.reserve(m_indices.size() * 3);
    for (int i = 0; i < m_indices.size(); ++i)
    {
        verticesVec.push_back(m_vertices[m_indices[i]].x);
        verticesVec.push_back(m_vertices[m_indices[i]].y);
        verticesVec.push_back(m_vertices[m_indices[i]].z);
    }
    return verticesVec;
}

std::vector<float> Plane::getTexCoordsArray()
{
    std::vector<float> texCoordsVec;
    texCoordsVec.reserve(m_indices.size() * 2);
    for (int i = 0; i < m_indices.size(); ++i)
    {
        texCoordsVec.push_back(m_texCoords[m_indices[i]].s);
        texCoordsVec.push_back(m_texCoords[m_indices[i]].t);
    }
    return texCoordsVec;
}

std::vector<float> Plane::getNormalsArray()
{
    std::vector<float> normalsVec;
    normalsVec.reserve(m_indices.size() * 3);
    for (int i = 0; i < m_indices.size(); ++i)
    {
        normalsVec.push_back(m_normals[m_indices[i]].x);
        normalsVec.push_back(m_normals[m_indices[i]].y);
        normalsVec.push_back(m_normals[m_indices[i]].z);
    }
    return normalsVec;
}

std::vector<float> Plane::getSTangentsArray()
{
    std::vector<float> tangentsVec;
    tangentsVec.reserve(m_indices.size() * 3);
    for (int i = 0; i < m_indices.size(); ++i)
    {
        tangentsVec.push_back(m_sTangents[m_indices[i]].x);
        tangentsVec.push_back(m_sTangents[m_indices[i]].y);
        tangentsVec.push_back(m_sTangents[m_indices[i]].z);
    }
    return tangentsVec;
}

std::vector<float> Plane::getTTangentsArray()
{
    std::vector<float> tangentsVec;
    tangentsVec.reserve(m_indices.size() * 3);
    for (int i = 0; i < m_indices.size(); ++i)
    {
        tangentsVec.push_back(m_tTangents[m_indices[i]].x);
        tangentsVec.push_back(m_tTangents[m_indices[i]].y);
        tangentsVec.push_back(m_tTangents[m_indices[i]].z);
    }
    return tangentsVec;
}

// match with glDrawElements
// if indices are supplied, use glDrawElements, else use glDrawArrays
bool Plane::supplyIndices()
{
    return true;
}

std::vector<int> Plane::getIndices()
{
    return m_indices;
}

std::vector<glm::vec3> Plane::getVertices()
{
    return m_vertices;
}

std::vector<glm::vec2> Plane::getTexCoords()
{
    return m_texCoords;
}

std::vector<glm::vec3> Plane::getNormals()
{
    return m_normals;
}

std::vector<glm::vec3> Plane::getSTangents()
{
    return m_sTangents;
}

std::vector<glm::vec3> Plane::getTTangents()
{
    return m_tTangents;
}

} // namespace Utils
