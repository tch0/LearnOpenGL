# reinforce surface details
# bump map
# normal map
# height map

opengl_instance(01BumpMap 01BumpMap.cpp)
copy_resources_after_build_target(01BumpMap
    ${CMAKE_CURRENT_SOURCE_DIR}/wood.png
)
opengl_instance(02NormalMap 02NormalMap.cpp)
copy_resources_after_build_target(02NormalMap
    ${CMAKE_CURRENT_SOURCE_DIR}/bricknormalmap.png
    ${CMAKE_CURRENT_SOURCE_DIR}/purewhite.png
)
opengl_instance(03HeightMap 03HeightMap.cpp)
copy_resources_after_build_target(03HeightMap
    ${CMAKE_CURRENT_SOURCE_DIR}/purewhite.png
    ${CMAKE_CURRENT_SOURCE_DIR}/height.jpg
)