# chapter 7 : lighting
# write a generic renderer program first, simplify everything
# lighting: flat shading / Gouraud shading / Phong shading
# light sources: global ambient light / directional light / point light / spotlight
# lighting with material and texture

opengl_instance(01TestRenderer 01TestRenderer.cpp)
copy_resources_after_build_target(01TestRenderer
    # model files
    ${CMAKE_CURRENT_SOURCE_DIR}/01TestModel.obj
    # texture files
    ${CMAKE_CURRENT_SOURCE_DIR}/01BrickTexture.jpg
)

opengl_instance(02GouraudShading 02GouraudShading.cpp)

opengl_instance(03SpotLight 03SpotLight.cpp)