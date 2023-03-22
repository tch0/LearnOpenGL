# chapter 8: shadows
# lighting with shadows
# the most usual way: shadow texture

opengl_instance(01ShadowTexture 01ShadowTexture.cpp)
copy_resources_after_build_target(01ShadowTexture
    ${CMAKE_CURRENT_SOURCE_DIR}/wood.png
)