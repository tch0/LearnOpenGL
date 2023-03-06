#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <exception>

namespace Utils
{

class NotImplementedFunction : public std::exception
{
private:
    std::string m_func;
public:
    NotImplementedFunction(const std::string& func) : m_func(func) {}
    [[nodiscard]] virtual char const* what() const noexcept override;
};

class Model
{
public:
    virtual ~Model() = 0;
    // match with glDrawArrays
    virtual bool supplyVertices();
    virtual bool supplyTexCoords();
    virtual bool supplyNormals();
    virtual bool supplyTangents();
    virtual std::vector<float> getVerticesArray();
    virtual std::vector<float> getTexCoordsArray();
    virtual std::vector<float> getNormalsArray();
    virtual std::vector<float> getSTangentsArray();
    virtual std::vector<float> getTTangentsArray();
    // match with glDrawElements
    // if indices are supplied, use glDrawElements, else use glDrawArrays
    virtual bool supplyIndices();
    virtual std::vector<int> getIndices();
    virtual std::vector<glm::vec3> getVertices();
    virtual std::vector<glm::vec2> getTexCoords();
    virtual std::vector<glm::vec3> getNormals();
    virtual std::vector<glm::vec3> getSTangents();
    virtual std::vector<glm::vec3> getTTangents();
};


} // namespace Utils