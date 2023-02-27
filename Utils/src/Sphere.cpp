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
    numVertices = (prec + 1) * (prec + 1);
    numIndices = prec * prec * 6;
    indices.resize(numIndices);
    vertices.resize(numVertices);
    texCoords.resize(numVertices);
    normals.resize(numVertices);

    // calculate triangle vertices
    for (int i = 0; i <= prec; i++)
    {
        for (int j = 0; j <= prec; j++)
        {
            float y = float(cos(glm::radians(180.f - i * 180.0f / prec)));
            float x = -float(cos(glm::radians(j*360.f / prec))) * float(abs(cos(asin(y))));
            float z = float(sin(glm::radians(j*360.f / prec))) * float(abs(cos(asin(y))));
            vertices[i*(prec+1) + j] = glm::vec3(x, y, z);
            texCoords[i*(prec+1) + j] = glm::vec2(float(j) / prec, float(i) / prec);
            normals[i*(prec+1) + j] = glm::vec3(x, y, z);
        }
    }

    // calculate triangle indices
    for (int i = 0; i < prec; i++)
    {
        for (int j = 0; j < prec; j++)
        {
            indices[6 * (i*prec + j) + 0] = i*(prec + 1) + j;
            indices[6 * (i*prec + j) + 1] = i*(prec + 1) + j + 1;
            indices[6 * (i*prec + j) + 2] = (i + 1)*(prec + 1) + j;
            indices[6 * (i*prec + j) + 3] = i*(prec + 1) + j + 1;
            indices[6 * (i*prec + j) + 4] = (i + 1)*(prec + 1) + j + 1;
            indices[6 * (i*prec + j) + 5] = (i + 1)*(prec + 1) + j;
        }
    }
}

std::vector<float> Sphere::getVerticesArray()
{
    std::vector<float> verticesVec;
    verticesVec.reserve(numIndices * 3);
    for (int i = 0; i < numIndices; ++i)
    {
        verticesVec.push_back(vertices[indices[i]].x);
        verticesVec.push_back(vertices[indices[i]].y);
        verticesVec.push_back(vertices[indices[i]].z);
    }
    return verticesVec;
}

std::vector<float> Sphere::getTexCoordsArray()
{
    std::vector<float> texCoordsVec;
    texCoordsVec.reserve(numIndices * 2);
    for (int i = 0; i < numIndices; ++i)
    {
        texCoordsVec.push_back(texCoords[indices[i]].s);
        texCoordsVec.push_back(texCoords[indices[i]].t);
    }
    return texCoordsVec;
}

std::vector<float> Sphere::getNormalsArray()
{
    std::vector<float> normalsVec;
    normalsVec.reserve(numIndices * 3);
    for (int i = 0; i < numIndices; ++i)
    {
        normalsVec.push_back(normals[indices[i]].x);
        normalsVec.push_back(normals[indices[i]].y);
        normalsVec.push_back(normals[indices[i]].z);
    }
    return normalsVec;
}

std::vector<int> Sphere::getIndices()
{
    return indices;
}

std::vector<glm::vec3> Sphere::getVertices()
{
    return vertices;
}

std::vector<glm::vec2> Sphere::getTexCoords()
{
    return texCoords;
}

std::vector<glm::vec3> Sphere::getNormals()
{
    return normals;
}


} // namespace Utils