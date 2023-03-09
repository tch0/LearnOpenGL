# chapter 7 : lighting
# write a generic renderer program first, simplify everything

opengl_instance(01TestRenderer 01TestRenderer.cpp)
copy_resources_after_build_target(01TestRenderer
    # model files
    ${CMAKE_CURRENT_SOURCE_DIR}/01TestModel.obj
    # texture files
    ${CMAKE_CURRENT_SOURCE_DIR}/01BrickTexture.jpg
)

opengl_instance(02GouraudShading 02GouraudShading.cpp)
