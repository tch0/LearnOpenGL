# geometry shader

opengl_instance(01ModifyPrimitive 01ModifyPrimitive.cpp)
copy_resources_after_build_target(01ModifyPrimitive
    ${CMAKE_CURRENT_SOURCE_DIR}/wood.png
)

opengl_instance(02DeletePrimitive 02DeletePrimitive.cpp)
copy_resources_after_build_target(02DeletePrimitive
    ${CMAKE_CURRENT_SOURCE_DIR}/wood.png
)

opengl_instance(03ChangePrimitiveType 03ChangePrimitiveType.cpp)