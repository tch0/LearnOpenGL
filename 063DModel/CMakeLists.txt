# chapter 6: 3D Model
# generate model through a program

opengl_instance(01GeneratedSphere 01GeneratedSphere.cpp)
copy_resources_after_build_target(01GeneratedSphere
    # shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/01Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/01Fragment.glsl
)

opengl_instance(02GeneratedTorus 02GeneratedTorus.cpp)
copy_resources_after_build_target(02GeneratedTorus
    # shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/02Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/02Fragment.glsl
    # textures
    ${CMAKE_CURRENT_SOURCE_DIR}/02BrickTexture.jpg
)