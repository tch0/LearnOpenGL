#pragma once
#include <vector>
#include <cmath>
#include <glm/glm.hpp>

namespace Utils
{

class Sphere
{
private:
    int numVertices;
    int numIndices;
    std::vector<int> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;

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