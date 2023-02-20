# chapter 2: OpenGL pipeline

opengl_instance(01JustAWindow 01JustAWindow.cpp)

opengl_instance(02Shaders 02Shaders.cpp)
# shaders
copy_glsl_sources_after_build_target(02Shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/02Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/02Fragment.glsl
)

opengl_instance(03Animation 03Animation.cpp)
copy_glsl_sources_after_build_target(03Animation
    ${CMAKE_CURRENT_SOURCE_DIR}/03Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/03Fragment.glsl
)