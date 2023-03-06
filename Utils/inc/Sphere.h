#pragma once
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <Model.h>

namespace Utils
{

class Sphere : public Model
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
    virtual bool supplyVertices() override;
    virtual bool supplyTexCoords() override;
    virtual bool supplyNormals() override;
    virtual std::vector<float> getVerticesArray() override;
    virtual std::vector<float> getTexCoordsArray() override;
    virtual std::vector<float> getNormalsArray() override;
    virtual bool supplyIndices() override;
    virtual std::vector<int> getIndices() override;
    virtual std::vector<glm::vec3> getVertices() override;
    virtual std::vector<glm::vec2> getTexCoords() override;
    virtual std::vector<glm::vec3> getNormals() override;
};

} // namespace Utils