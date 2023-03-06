#pragma once
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include "Model.h"

namespace Utils
{

class Torus : public Model
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
    virtual bool supplyVertices() override;
    virtual bool supplyTexCoords() override;
    virtual bool supplyNormals() override;
    virtual bool supplyTangents() override;
    virtual std::vector<float> getVerticesArray() override;
    virtual std::vector<float> getTexCoordsArray() override;
    virtual std::vector<float> getNormalsArray() override;
    virtual std::vector<float> getSTangentsArray() override;
    virtual std::vector<float> getTTangentsArray() override;

    // match with glDrawElements
    virtual bool supplyIndices() override;
    virtual std::vector<int> getIndices() override;
    virtual std::vector<glm::vec3> getVertices() override;
    virtual std::vector<glm::vec2> getTexCoords() override;
    virtual std::vector<glm::vec3> getNormals() override;
    virtual std::vector<glm::vec3> getSTangents() override;
    virtual std::vector<glm::vec3> getTTangents() override;
};

} // namespace Utils