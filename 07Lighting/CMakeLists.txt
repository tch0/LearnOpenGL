# chapter 7 : lighting
# write a generic renderer program first, simplify everything
# lighting: flat shading / Gouraud shading / Phong shading
# light sources: global ambient light / directional light / point light / spotlight
# lighting with material and texture

opengl_instance(01TestRenderer 01TestRenderer.cpp)
copy_resources_after_build_target(01TestRenderer
    # model files
    ${CMAKE_CURRENT_SOURCE_DIR}/TestModel.obj
    # texture files
    ${CMAKE_CURRENT_SOURCE_DIR}/BrickTexture.jpg
)

opengl_instance(02GouraudShading 02GouraudShading.cpp)

opengl_instance(03SpotLight 03SpotLight.cpp)

opengl_instance(04PhongShading 04PhongShading.cpp)

opengl_instance(05GouraudWithTexture 05GouraudWithTexture.cpp)
copy_resources_after_build_target(05GouraudWithTexture
    # texture files
    ${CMAKE_CURRENT_SOURCE_DIR}/BrickTexture.jpg
)

opengl_instance(06PhongWithTexture 06PhongWithTexture.cpp)
copy_resources_after_build_target(06PhongWithTexture
    # texture files
    ${CMAKE_CURRENT_SOURCE_DIR}/BrickTexture.jpg
)

opengl_instance(07MixMaterialAndTexture 07MixMaterialAndTexture.cpp)
copy_resources_after_build_target(07MixMaterialAndTexture
    # texture files
    ${CMAKE_CURRENT_SOURCE_DIR}/BrickTexture.jpg
)