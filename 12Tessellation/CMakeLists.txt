# tessellation shader

opengl_instance(01TessExample 01TessExample.cpp)
opengl_instance(02BezierTessellation 02BezierTessellation.cpp)
copy_resources_after_build_target(02BezierTessellation
    ${CMAKE_CURRENT_SOURCE_DIR}/wood.png
)