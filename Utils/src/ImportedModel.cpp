#include <ImportedModel.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <Logger.h>

namespace Utils
{


bool ImportedModel::supplyVertices()
{
    return true;
}

bool ImportedModel::supplyTexCoords()
{
    return true;
}
bool ImportedModel::supplyNormals()
{
    return true;
}

ImportedModel::ImportedModel(const char* filePath)
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

    std::ifstream fin(filePath);
    if (!fin.is_open())
    {
        Logger::globalLogger().warning("Model file "s + filePath + " does not exist!"s);
        return;
    }
    std::string line;
    float x, y, z;
    std::vector<int> vertIndices, tcIndices, normalIndices;
    while (!fin.eof())
    {
        std::getline(fin, line);
        if (line.compare(0, 2, "v ") == 0)
        {
            std::istringstream iss(line.erase(0, 1));
            iss >> x;
            iss >> y;
            iss >> z;
            vertices.push_back(glm::vec3(x, y, z));
        }
        else if (line.compare(0, 2, "vt") == 0)
        {
            std::istringstream iss(line.erase(0, 2));
            iss >> x;
            iss >> y;
            texCoords.push_back(glm::vec2(x, y));
        }
        else if (line.compare(0, 2, "vn") == 0)
        {
            std::istringstream iss(line.erase(0, 2));
            iss >> x;
            iss >> y;
            iss >> z;
            normals.push_back(glm::vec3(x, y, z));
        }
        else if (line.compare(0, 2, "f ") == 0) 
        {
            std::string oneVertex, v, t, n;
            std::istringstream iss(line.erase(0, 2));
            for (int i = 0; i < 3; ++i)
            {
                std::getline(iss, oneVertex, ' ');
                std::istringstream oneVertexIss(oneVertex);
                std::getline(oneVertexIss, v, '/');
                std::getline(oneVertexIss, t, '/');
                std::getline(oneVertexIss, n, '/');

                if (v.empty() || t.empty() || n.empty())
                {
                    Logger::globalLogger().warning("Object file "s + filePath + " parse error: none of vertex/texture coordinate/normal could be empty."s);
                    return;
                }

                int vertIdx = std::stoi(v) - 1;
                int tcIdx = std::stoi(t) - 1;
                int normIdx = std::stoi(n) - 1;
                vertIndices.push_back(vertIdx);
                tcIndices.push_back(tcIdx);
                normalIndices.push_back(normIdx);
            }
        }
    }
    for (auto idx : vertIndices)
    {
        m_vertices.push_back(vertices[idx].x);
        m_vertices.push_back(vertices[idx].y);
        m_vertices.push_back(vertices[idx].z);
    }
    for (auto idx : tcIndices)
    {
        m_texCoords.push_back(texCoords[idx].s);
        m_texCoords.push_back(texCoords[idx].t);
    }
    for (auto idx : normalIndices)
    {
        m_normals.push_back(normals[idx].x);
        m_normals.push_back(normals[idx].y);
        m_normals.push_back(normals[idx].z);
    }
}

std::vector<float> ImportedModel::getVerticesArray()
{
    return m_vertices;
}

std::vector<float> ImportedModel::getTexCoordsArray()
{
    return m_texCoords;
}

std::vector<float> ImportedModel::getNormalsArray()
{
    return m_normals;
}

} // namespace Utils