#include <glad/gl.h> // glad2!
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <soil2/SOIL2.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cassert>
#include <string>
#include <fstream>
#include <memory>
#include <Utils.h>
#include <Model.h>
#include <Sphere.h>
#include <Torus.h>
#include <Renderer.h>
#include <ImportedModel.h>

class Axises : public Utils::Model
{
private:
    float m_Len;
public:
    Axises(float len) : m_Len(len) {}
    virtual bool supplyVertices() override
    {
        return true;
    }
    virtual std::vector<float> getVerticesArray() override
    {
        std::vector<float> ret = {
            -1.0f * m_Len, 0.0f, 0.0f, m_Len, 0.0f, 0.0f,
            0.0f, -1.0f * m_Len, 0.0f, 0.0f, m_Len, 0.0f,
            0.0f, 0.0f, -1.0f * m_Len, 0.0f, 0.0f, m_Len
        };
        return ret;
    };
};

int main(int argc, char const *argv[])
{
    Utils::Renderer renderer("TestRenderer");
    
    // sphere
    std::shared_ptr<Utils::Model> spSphere(new Utils::Sphere());
    auto sphereIdx = renderer.addModel(spSphere, Utils::Renderer::PureColorPoints);
    renderer.setModelRotation(sphereIdx, glm::vec3(0.0f, 1.0f, 0.0f), 1.0f);
    renderer.setColor(sphereIdx, glm::vec4(1.0f, 0.5f, 0.5f, 1.0f));

    // torus
    std::shared_ptr<Utils::Model> spTorus(new Utils::Torus(1.0f, 3.0f));
    auto torusIdx = renderer.addModel(spTorus, Utils::Renderer::SpecificTexture);
    renderer.setModelRotation(torusIdx, glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
    renderer.setTexture(torusIdx, "BrickTexture.jpg");

    // imported model
    std::shared_ptr<Utils::Model> spImportedModel(new Utils::ImportedModel("TestModel.obj"));
    auto importIdx = renderer.addModel(spImportedModel, Utils::Renderer::PureColorLineStrip);
    renderer.setModelRotation(importIdx, glm::vec3(0.0f, 1.0f, 0.0f), -0.5f);

    // draw user-defined axises, override renderer axises
    std::shared_ptr<Utils::Model> spAxis(new Axises(100.0f));
    renderer.addModel(spAxis, Utils::Renderer::VaryingColorLines);

    // run the render loop
    renderer.run();
    return 0;
}