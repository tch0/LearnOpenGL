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
    Utils::Renderer renderer("02EnvironmentMap");
    renderer.enableSkyBox("right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg");

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
    std::shared_ptr<Utils::Model> spSphere(new Utils::Sphere());
    auto sphereIdx = renderer.addModel(spSphere, Utils::Renderer::EnvironmentMap);
    
    std::shared_ptr<Utils::Model> spTorus(new Utils::Torus(1, 3));
    auto torusIdx = renderer.addModel(spTorus, Utils::Renderer::EnvironmentMap);
    // renderer.setModelRotation(torusIdx, glm::vec3(1.0f, 0.0f, 0.0f), 0.5);

    renderer.run();
    return 0;
}
