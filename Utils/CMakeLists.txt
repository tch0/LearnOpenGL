# common utilities for all OpenGL programs
# static library

add_library(Utils STATIC src/Utils.cpp)
target_include_directories(Utils
    PUBLIC
        ${CMAKE_SOURCE_DIR}/3rdparty-install/include
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)
target_link_libraries(Utils
    PRIVATE general_cxx_compiler_flags
)