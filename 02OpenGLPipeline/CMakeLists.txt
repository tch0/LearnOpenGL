# chapter 2: OpenGL pipeline

opengl_instance(JustAWindow 01JustAWindow.cpp)

opengl_instance(Shaders 02Shaders.cpp)
# shaders
copy_glsl_sources_after_build_target(Shaders
    ${CMAKE_CURRENT_SOURCE_DIR}/02Vertex.glsl
    ${CMAKE_CURRENT_SOURCE_DIR}/02Fragment.glsl
)
