# chapter 4 : manage 3d graphics data
# everything in chpater 3
# vertex array obejct, vertex buffer object
# render multiple obejcts
# matrix stack to build complicated scene
# back face culling of OpenGL

opengl_instance(01Cube3d 01Cube3d.cpp)
copy_resources_after_build_target(01Cube3d
    ${CMAKE_CURRENT_SOURCE_DIR}/01Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/01Fragment.glsl
)

opengl_instance(02Multiple3dCubes 02Multiple3dCubes.cpp)
copy_resources_after_build_target(01Cube3d
    ${CMAKE_CURRENT_SOURCE_DIR}/02Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/02Fragment.glsl
)

opengl_instance(03MultipleModels 03MultipleModels.cpp)
copy_resources_after_build_target(03MultipleModels
    ${CMAKE_CURRENT_SOURCE_DIR}/03Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/03Fragment.glsl
)

opengl_instance(04MatrixStack 04MatrixStack.cpp)
copy_resources_after_build_target(03MultipleModels
    ${CMAKE_CURRENT_SOURCE_DIR}/04Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/04Fragment.glsl
)