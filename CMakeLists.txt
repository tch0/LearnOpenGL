cmake_minimum_required(VERSION 3.15)

project (LearnOpenGL
    DESCRIPTION "Learn about OpenGL"
    VERSION "0.0.1"
    LANGUAGES CXX
)

# general compiler flags, C++ standard, warnings, etc.
add_library(general_cxx_compiler_flags INTERFACE)
# C++ standard
target_compile_features(general_cxx_compiler_flags INTERFACE cxx_std_11)
# for different toolchain
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
# warning options for different toolchain
target_compile_options(general_cxx_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-pedantic-errors;-Wformat=2;-Wno-unused-parameter>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:/W3>>"
)

# common utility library for all programs
add_subdirectory(Utils)

# 3rd-party libraries:
#       static library: glfw glad soil2
#       header only: glm
list(APPEND 3rdparty_libs glad glfw3 soil2)

# other essential libraries
if (MINGW OR MSVC)
    list(APPEND essential_libs gdi32 opengl32)
else() # for Unix
    list(APPEND essential_libs GL X11 pthread dl)
endif()

# configuration of an executable
function(opengl_instance target)
    add_executable(${ARGV})
    target_include_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/3rdparty-install/include)
    target_link_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/3rdparty-install/lib)
    target_link_libraries(${target}
        PRIVATE
            general_cxx_compiler_flags
        PUBLIC
            ${3rdparty_libs}
            ${essential_libs}
            Utils
    )
endfunction()

# copy glsl sources used by executable to its corresponding binary output directory after build target.
function(copy_resources_after_build_target target sources)
    set(sources ${ARGV})
    list(POP_FRONT sources)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${target}> ${sources}
        COMMAND_EXPAND_LISTS
    )
endfunction()

# examples
add_subdirectory(02OpenGLPipeline)
add_subdirectory(03Mathematics)
add_subdirectory(04Manage3dGraphicsData)