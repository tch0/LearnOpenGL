# texture
# texture object, texture coordinates
# mipmapping
# anisotropic filtering
# repeat, clamp
# perspective correction (noperspective keyword in GLSL)

opengl_instance(01TexturePyramid 01TexturePyramid.cpp)
copy_resources_after_build_target(01TexturePyramid
    # shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/01Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/01Fragment.glsl
    # textures
    ${CMAKE_CURRENT_SOURCE_DIR}/01BrickTExture.jpg
)

opengl_instance(02MipMapping 02MipMapping.cpp)
copy_resources_after_build_target(02MipMapping
    # shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/02Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/02Fragment.glsl
    # textures
    ${CMAKE_CURRENT_SOURCE_DIR}/02BlackWhiteGrid.jpg
)
