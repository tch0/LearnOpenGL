#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "Model.h"

namespace Utils
{

class ImportedModel : public Model
{
private:
    std::vector<float> m_vertices;
    std::vector<float> m_texCoords;
    std::vector<float> m_normals;
public: 
    ImportedModel(const char* filePath);
    // match with glDrawArrays
    virtual bool supplyVertices() override;
    virtual bool supplyTexCoords() override;
    virtual bool supplyNormals() override;
    virtual std::vector<float> getVerticesArray() override;
    virtual std::vector<float> getTexCoordsArray() override;
    virtual std::vector<float> getNormalsArray() override;
};


} // namespace Utils