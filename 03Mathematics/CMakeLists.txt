# chapter 2: basic mathematics
# point, vector, matrix
# tranformations: translation, scaling, rotation
# model, view, projection

# example of transformation, based on 02OpenGLPipeline/03Animation.cpp
opengl_instance(01RotateTriangle 01RotateTriangle.cpp)
copy_resources_after_build_target(01RotateTriangle
    ${CMAKE_CURRENT_SOURCE_DIR}/01Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/01Fragment.glsl
)