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
#include <Plane.h>
#include <Logger.h>

int main(int argc, char const *argv[])
{
    Utils::Renderer renderer("02PCF");
    renderer.setPCFMode(Utils::Renderer::Sample4Dithered, 2.5f); // soft shadow

    // light sources 
    renderer.setGlobalAmbientLight(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)); 
    // directional light
    renderer.addDirectionalLight(glm::vec4(0.0f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
    // pure white point light
    renderer.addPointLight(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec3(5.0f, 5.0f, 5.0f), 1.0f, 0.002f, 0.0003f);
    renderer.addSpotLight(glm::vec4(0.0f), glm::vec4(0.7f, 0.7f, 0.7f, 0.7f), glm::vec4(0.7f, 0.7f, 0.7f, 0.7f),
        glm::vec3(0.0f, 10.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::pi<float>() / 6.0f, 5.0f);

    // models
    std::shared_ptr<Utils::Model> spPlane(new Utils::Plane(-2.0f, 100.f, 20.0f, 20));
    auto planeIdx = renderer.addModel(spPlane, Utils::Renderer::PhongShadingWithShadow);
    // renderer.setMaterial(planeIdx, Utils::silverMaterial, 0.5);
    renderer.setTexture(planeIdx, "wood.png", 1.0);
    renderer.setLightingMode(planeIdx, Utils::Renderer::PhongShading);

    std::shared_ptr<Utils::Model> spSphere(new Utils::Sphere());
    auto sphereIdx = renderer.addModel(spSphere, Utils::Renderer::PhongShadingWithShadow);
    // renderer.setMaterial(sphereIdx, Utils::goldMaterial);
    renderer.setTexture(sphereIdx, "wood.png", 1.0);
    renderer.setLightingMode(sphereIdx, Utils::Renderer::PhongShading);
    
    std::shared_ptr<Utils::Model> spTorus(new Utils::Torus(1, 3));
    auto torusIdx = renderer.addModel(spTorus, Utils::Renderer::PhongShadingWithShadow);
    // renderer.setMaterial(torusIdx, Utils::bronzeMaterial);
    renderer.setTexture(torusIdx, "wood.png", 1.0);
    renderer.setLightingMode(torusIdx, Utils::Renderer::PhongShading);
    renderer.setModelRotation(torusIdx, glm::vec3(1.0f, 0.0f, 0.0f), 0.5);

    renderer.run();
    return 0;
}
