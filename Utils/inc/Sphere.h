#pragma once
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

namespace Utils
{

class Sphere
{
private:
    int m_numVertices;
    int m_numIndices;
    std::vector<int> m_indices;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_texCoords;
    std::vector<glm::vec3> m_normals;

    void init(int);
public:
    Sphere(int prec = 48);
    std::vector<float> getVerticesArray();
    std::vector<float> getTexCoordsArray();
    std::vector<float> getNormalsArray();

    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();
};

} // namespace Utils