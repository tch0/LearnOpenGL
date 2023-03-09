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
#include <Logger.h>

using namespace std::string_literals;

int main(int argc, char const *argv[])
{
    
    Utils::Renderer renderer("02GouraudShading");

    // light sources
    renderer.setGlobalAmbientLight(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));
    // pure white point light
    renderer.addPointLight(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec3(2.0f, 2.0f, 2.0f), 1.0f, 0.002f, 0.0003f);

    // models
    std::shared_ptr<Utils::Model> spSphere(new Utils::Sphere());
    auto sphereIdx = renderer.addModel(spSphere, Utils::Renderer::LightingMaterial);
    renderer.setMaterial(sphereIdx, Utils::goldMaterial);
    renderer.setLightingMode(sphereIdx, Utils::Renderer::FlatShading);
    
    std::shared_ptr<Utils::Model> spTorus(new Utils::Torus(1, 3));
    auto torusIdx = renderer.addModel(spTorus, Utils::Renderer::LightingMaterial);
    renderer.setMaterial(torusIdx, Utils::bronzeMaterial);
    renderer.setLightingMode(torusIdx, Utils::Renderer::GouraudShading);

    renderer.run();
    return 0;
}
