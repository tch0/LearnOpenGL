# reinforce surface details
# bump map
# normal map
# height map

opengl_instance(01BumpMap 01BumpMap.cpp)
copy_resources_after_build_target(01BumpMap
    ${CMAKE_CURRENT_SOURCE_DIR}/wood.png
)
