cmake_minimum_required(VERSION 3.15)

project (LearnOpenGL
    DESCRIPTION "Learn about OpenGL"
    VERSION "0.0.1"
    LANGUAGES CXX
)

include(CheckCXXSourceCompiles)

# general compiler flags, C++ standard, warnings, etc.
add_library(general_cxx_compiler_flags INTERFACE)
# C++ standard
target_compile_features(general_cxx_compiler_flags INTERFACE cxx_std_20)
# for different toolchain
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
# warning options for different toolchain
target_compile_options(general_cxx_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-pedantic-errors;-Wformat=2;-Wno-unused-parameter>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:/W3>>"
)

# extra compiler flags, disable inevitable annoying third-party library headers' warnings
add_library(extra_cxx_compiler_flags INTERFACE)
target_compile_options(extra_cxx_compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wno-volatile>>" # for glm headers
)

# check format is supported or not on your compiler, use fmt library if not supported yet!
add_library(format_bridge INTERFACE)
check_cxx_source_compiles("#include <format>\nint main() { return 0; }" format_supported)
if (NOT format_supported)
    target_include_directories(format_bridge INTERFACE ${CMAKE_SOURCE_DIR}/3rdparty-install/include/formatbridge)
    message("## <format> is not support on your compiler yet, use fmt library instead!")
endif()

# 3rd-party libraries:
#       static library: glfw glad soil2
#       header only: glm
list(APPEND 3rdparty_libs glad glfw3 soil2)

# other essential libraries
if (MINGW OR MSVC)
    list(APPEND essential_libs gdi32 opengl32)
elseif(UNIX) # for Unix
    list(APPEND essential_libs GL X11 pthread dl)
endif()

# define a internal static library
function(define_a_static_lib target sources)
    set(sources ${ARGV})
    list(POP_FRONT sources)
    add_library(${target} STATIC ${sources})
    target_include_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/3rdparty-install/include)
    target_link_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/3rdparty-install/lib)
    target_link_libraries(${target}
        PRIVATE
            general_cxx_compiler_flags
            extra_cxx_compiler_flags
        PUBLIC
            ${3rdparty_libs}
            ${essential_libs}
            format_bridge
    )
endfunction()

# configuration of an executable
function(opengl_instance target)
    add_executable(${ARGV})
    target_include_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/3rdparty-install/include)
    target_link_directories(${target} PUBLIC ${CMAKE_SOURCE_DIR}/3rdparty-install/lib)
    target_link_libraries(${target}
        PRIVATE
            general_cxx_compiler_flags
            extra_cxx_compiler_flags
        PUBLIC
            Utils # the link order matters on gcc, Utils depends on 3rdparty libraries, so Utils must be ahead of 3rd parties
            ${3rdparty_libs}
            ${essential_libs}
            format_bridge
    )
endfunction()

# copy resources(glsl files, texture files, etc.) used by executable to its corresponding binary output directory after build target.
function(copy_resources_after_build_target target sources)
    set(sources ${ARGV})
    list(POP_FRONT sources)
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${target}> ${sources}
        COMMAND_EXPAND_LISTS
    )
endfunction()

# common utility library for all programs
add_subdirectory(Utils)
# examples
add_subdirectory(02OpenGLPipeline)
add_subdirectory(03Mathematics)
add_subdirectory(04Manage3dGraphicsData)
add_subdirectory(05Texture)
add_subdirectory(063DModel)
add_subdirectory(07Lighting)
add_subdirectory(08Shadows)
add_subdirectory(09SkyBox)
add_subdirectory(10SurfaceDetails)
add_subdirectory(12Tessellation)
add_subdirectory(13GeometryShader)