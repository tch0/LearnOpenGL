# sky box
opengl_instance(01SkyBox 01SkyBox.cpp)
copy_resources_after_build_target(01SkyBox
    ${CMAKE_CURRENT_SOURCE_DIR}/front.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/back.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/top.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/bottom.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/left.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/right.jpg
    ${CMAKE_CURRENT_SOURCE_DIR}/wood.png
)