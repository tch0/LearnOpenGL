#pragma once

#include "Model.h"


namespace Utils
{

class Plane : public Model
{
private:
    std::vector<int> m_indices;
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec2> m_texCoords;
    std::vector<glm::vec3> m_normals;
    std::vector<glm::vec3> m_sTangents; // the tangent of texture s direction
    std::vector<glm::vec3> m_tTangents; // the tangent of texture t direction
public:
    // orthogonal to y axis
    Plane(float y, float width = 100.0f, int precision = 50);
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
    // if indices are supplied, use glDrawElements, else use glDrawArrays
    virtual bool supplyIndices() override;
    virtual std::vector<int> getIndices() override;
    virtual std::vector<glm::vec3> getVertices() override;
    virtual std::vector<glm::vec2> getTexCoords() override;
    virtual std::vector<glm::vec3> getNormals() override;
    virtual std::vector<glm::vec3> getSTangents() override;
    virtual std::vector<glm::vec3> getTTangents() override;
};


} // namespace Utils
