cmake_minimum_required(VERSION 3.15)

project(3rdparty)

add_subdirectory(glfw)
add_subdirectory(glad)
add_subdirectory(glm)
add_subdirectory(SOIL2)
add_subdirectory(fmt)

set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty-install)

# install glm headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/glm/glm DESTINATION include)
# install formatbridge
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/formatbridge DESTINATION include)