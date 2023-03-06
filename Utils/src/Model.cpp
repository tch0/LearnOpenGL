#include <Model.h>

namespace Utils
{


// =======================================NotImplementedFunction===================================

char const* NotImplementedFunction::what() const noexcept
{
    static std::string prompt;
    prompt = m_func + " not implemented!";
    return prompt.c_str();
}


// =======================================Model====================================================
Model::~Model()
{
}

bool Model::supplyVertices()
{
    return false;
}

bool Model::supplyTexCoords()
{
    return false;
}

bool Model::supplyNormals()
{
    return false;
}

bool Model::supplyTangents()
{
    return false;
}

std::vector<float> Model::getVerticesArray()
{
    throw NotImplementedFunction("Model::getVerticesArray");
}

std::vector<float> Model::getTexCoordsArray()
{
    throw NotImplementedFunction("Model::getTexCoordsArray");
}

std::vector<float> Model::getNormalsArray()
{
    throw NotImplementedFunction("Model::getNormalsArray");
}

std::vector<float> Model::getSTangentsArray()
{
    throw NotImplementedFunction("Model::getSTangentsArray");
}

std::vector<float> Model::getTTangentsArray()
{
    throw NotImplementedFunction("Model::getTTangentsArray");
}

bool Model::supplyIndices()
{
    return false;
}

// match with glDrawElements
std::vector<int> Model::getIndices()
{
    throw NotImplementedFunction("Model::getIndices");
}

std::vector<glm::vec3> Model::getVertices()
{
    throw NotImplementedFunction("Model::getVertices");
}

std::vector<glm::vec2> Model::getTexCoords()
{
    throw NotImplementedFunction("Model::getTexCoords");
}

std::vector<glm::vec3> Model::getNormals()
{
    throw NotImplementedFunction("Model::getNormals");
}

std::vector<glm::vec3> Model::getSTangents()
{
    throw NotImplementedFunction("Model::getSTangents");
}

std::vector<glm::vec3> Model::getTTangents()
{
    throw NotImplementedFunction("Model::getTTangents");
}


} // namespace Utils