#include <Torus.h>
#include <glm/ext.hpp>
#include <cmath>

namespace Utils
{

Torus::Torus(float cRadius, float rRadius, int precision)
    : m_circleRadius(cRadius)
    , m_ringRadius(rRadius)
{
    init(precision);
}

void Torus::init(int prec)
{
    m_numVertices = (prec + 1) * (prec + 1);
    m_numIndices = prec * prec * 6;
    m_indices.resize(m_numIndices);
    m_vertices.resize(m_numVertices);
    m_texCoords.resize(m_numVertices);
    m_normals.resize(m_numVertices);
    m_sTangents.resize(m_numVertices);
    m_tTangents.resize(m_numVertices);

    // calculate triangle vertices
    for (int i = 0; i <= prec; i++) // the circle index
    {
        // inner circle:
        //  x = sqrt(m_circleRadius) * cos(angle) + m_ringRadius;
        //  y = sqrt(m_circleRadius) * sin(angle);
        //  z = 0;
        float r = sqrt(m_circleRadius);
        float angle = 2.0f * glm::pi<float>() / prec * i;
        float x = r * cos(angle) + m_ringRadius;
        float y = r * sin(angle);
        float z = 0.0f;
        glm::vec3 sTan(0.0f, 0.0f, -1.0f); // around the ring
        glm::vec3 tTan(-sin(angle), cos(angle), 0.0f); // around the circle
        glm::vec3 normal(cos(angle), sin(angle), 0.0f);

        // outter ring
        for (int j = 0; j <= prec; j++) // the ring index, rotate around the y-axis
        {
            float angle2 = 2.0f * glm::pi<float>() / prec * j;
            glm::mat4 transMat = glm::rotate(glm::mat4(1.0f), angle2, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::vec3 vertexRes = glm::vec4(x, y, z, 1.0f) * transMat;
            glm::vec2 texCoordRes = glm::vec2(j * 1.0f / prec, i * 1.0f / prec);
            glm::vec3 sTanRes = glm::vec4(sTan, 1.0f) * transMat;
            glm::vec3 tTanRes = glm::vec4(tTan, 1.0f) * transMat;
            glm::vec3 normalRes = glm::vec4(normal, 1.0f) * transMat;
            m_vertices[i * (prec + 1) + j] = vertexRes;
            m_texCoords[i * (prec + 1) + j] = texCoordRes;
            m_normals[i * (prec + 1) + j] = normalRes;
            m_sTangents[i * (prec + 1) + j] = sTanRes;
            m_tTangents[i * (prec + 1) + j] = tTanRes;
        }
    }

    // calculate triangle indices
    for (int i = 0; i < prec; i++)
    {
        for (int j = 0; j < prec; j++)
        {
            // m_indices[6 * (i*prec + j) + 0] = i*(prec + 1) + j;
            // m_indices[6 * (i*prec + j) + 1] = i*(prec + 1) + j + 1;
            // m_indices[6 * (i*prec + j) + 2] = (i + 1)*(prec + 1) + j;
            // m_indices[6 * (i*prec + j) + 3] = i*(prec + 1) + j + 1;
            // m_indices[6 * (i*prec + j) + 4] = (i + 1)*(prec + 1) + j + 1;
            // m_indices[6 * (i*prec + j) + 5] = (i + 1)*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 0] = i*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 1] = (i + 1)*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 2] = i*(prec + 1) + j + 1;
            m_indices[6 * (i*prec + j) + 3] = i*(prec + 1) + j + 1;
            m_indices[6 * (i*prec + j) + 4] = (i + 1)*(prec + 1) + j;
            m_indices[6 * (i*prec + j) + 5] = (i + 1)*(prec + 1) + j + 1;
        }
    }
}

std::vector<float> Torus::getVerticesArray()
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

std::vector<float> Torus::getTexCoordsArray()
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

std::vector<float> Torus::getNormalsArray()
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

std::vector<float> Torus::getSTangentsArray()
{
    std::vector<float> tangentsVec;
    tangentsVec.reserve(m_numIndices * 3);
    for (int i = 0; i < m_numIndices; ++i)
    {
        tangentsVec.push_back(m_sTangents[m_indices[i]].x);
        tangentsVec.push_back(m_sTangents[m_indices[i]].y);
        tangentsVec.push_back(m_sTangents[m_indices[i]].z);
    }
    return tangentsVec;
}

std::vector<float> Torus::getTTangentsArray()
{
    std::vector<float> tangentsVec;
    tangentsVec.reserve(m_numIndices * 3);
    for (int i = 0; i < m_numIndices; ++i)
    {
        tangentsVec.push_back(m_tTangents[m_indices[i]].x);
        tangentsVec.push_back(m_tTangents[m_indices[i]].y);
        tangentsVec.push_back(m_tTangents[m_indices[i]].z);
    }
    return tangentsVec;
}

std::vector<int> Torus::getIndices()
{
    return m_indices;
}

std::vector<glm::vec3> Torus::getVertices()
{
    return m_vertices;
}

std::vector<glm::vec2> Torus::getTexCoords()
{
    return m_texCoords;
}

std::vector<glm::vec3> Torus::getNormals()
{
    return m_normals;
}

std::vector<glm::vec3> Torus::getSTangents()
{
    return m_sTangents;
}

std::vector<glm::vec3> Torus::getTTangents()
{
    return m_tTangents;
}

} // namespace Utils