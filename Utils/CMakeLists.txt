# common utilities for all OpenGL programs
# static library
#   common utils:
#       error handling
#       shader loading
#       texture utilities
#   some model generator:
#       Sphere
#       Torus
#   model import utlity:
#       OBJ file reader
#   a simple renderer implementation

file(GLOB utils_sources src/*.cpp)
define_a_static_lib(Utils ${utils_sources})
# Utils headers
target_include_directories(Utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)