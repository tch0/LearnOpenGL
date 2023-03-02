#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace Utils
{

class ImportedModel
{
private:
    std::vector<float> m_vertices;
    std::vector<float> m_texCoords;
    std::vector<float> m_normals;
public: 
    ImportedModel(const char* filePath);
    // match with glDrawArrays
    std::vector<float> getVerticesArray();
    std::vector<float> getTexCoordsArray();
    std::vector<float> getNormalsArray();
};


} // namespace Utils