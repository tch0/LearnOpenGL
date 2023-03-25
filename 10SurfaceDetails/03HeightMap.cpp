#include <Renderer.h>
#include <Sphere.h>
#include <Torus.h>
#include <Plane.h>

int main(int argc, char const *argv[])
{
    Utils::Renderer renderer("03HeightMap.cpp");
    renderer.setFaceCullingAttribute(false);

    // light sources 
    renderer.setGlobalAmbientLight(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f)); 
    // directional light
    renderer.addDirectionalLight(glm::vec4(0.0f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f), glm::vec4(0.6f, 0.6f, 0.6f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f));
    // // pure white point light
    renderer.addPointLight(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
        glm::vec3(5.0f, 5.0f, 5.0f), 1.0f, 0.002f, 0.0003f);
    // renderer.addSpotLight(glm::vec4(0.0f), glm::vec4(0.7f, 0.7f, 0.7f, 0.7f), glm::vec4(0.7f, 0.7f, 0.7f, 0.7f),
    //     glm::vec3(0.0f, 10.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::pi<float>() / 6.0f, 5.0f);

    // models
    std::shared_ptr<Utils::Model> spPlane(new Utils::Plane(0.0f, 100.0f, 2.0f, 50));
    auto planeIdx = renderer.addModel(spPlane, Utils::Renderer::LightingMaterialTexture);
    renderer.setLightingMode(planeIdx, Utils::Renderer::PhongShading);
    renderer.setTexture(planeIdx, "purewhite.png");
    renderer.setHeightMap(planeIdx, "height.jpg", 10.0f);
    renderer.setNormalMap(planeIdx, "bricknormalmap.png");
    
    renderer.run();
    return 0;
}
