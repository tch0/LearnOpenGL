#pragma once
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

namespace Utils
{

class Torus
{
private:
    float m_circleRadius;
    float m_ringRadius;
    int m_numVertices;
    int m_numIndices;
    std::vector<int> m_indices;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_texCoords;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_sTangents; // the tangent of texture s direction
    std::vector<glm::vec3> m_tTangents; // the tangent of texture t direction

    void init(int precision);
public:
    Torus(float cRadius, float rRadius, int precision = 48);
    // match with glDrawArrays
    std::vector<float> getVerticesArray();
    std::vector<float> getTexCoordsArray();
    std::vector<float> getNormalsArray();
    std::vector<float> getSTangentsArray();
    std::vector<float> getTTangentsArray();

    // match with glDrawElements
    std::vector<int> getIndices();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec2> getTexCoords();
    std::vector<glm::vec3> getNormals();
    std::vector<glm::vec3> getSTangents();
    std::vector<glm::vec3> getTTangents();
};

} // namespace Utils