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

// mix texture and material, given each a specific weight

int main(int argc, char const *argv[])
{
    Utils::Renderer renderer("06PhongWithTexture");
    // models
    std::shared_ptr<Utils::Model> spTorus(new Utils::Torus(1, 3));
    auto torusIdx = renderer.addModel(spTorus, Utils::Renderer::LightingMaterialTexture);
    renderer.setTexture(torusIdx, "BrickTexture.jpg", 0.5);
    renderer.setMaterial(torusIdx, Utils::goldMaterial, 0.5);
    renderer.setLightingMode(torusIdx, Utils::Renderer::PhongShading);

    // lights
    renderer.setGlobalAmbientLight(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    renderer.addPointLight(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec3(2.0f, 0.5f, 2.0f), 1.0f, 0.02f, 0.003f);
    renderer.addDirectionalLight(glm::vec4(0.0f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
    renderer.addSpotLight(glm::vec4(0.0f), glm::vec4(0.7f, 0.7f, 0.7f, 0.7f), glm::vec4(0.7f, 0.7f, 0.7f, 0.7f),
        glm::vec3(2.0f, 5.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::pi<float>() / 20.0f, 5.0f);

    renderer.run();
    return 0;
}
