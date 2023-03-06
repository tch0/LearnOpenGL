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
add_library(Utils STATIC ${utils_sources})

target_include_directories(Utils
    PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdparty-install/include
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_directories(Utils
    INTERFACE ${CMAKE_SOURCE_DIR}/3rdparty-install/lib
)
target_link_libraries(Utils
    PRIVATE general_cxx_compiler_flags
    INTERFACE glad
)