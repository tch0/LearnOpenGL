#version 430

in vec2 tc; // texture coordinates input, the interpolation result from rasterizer
out vec4 color;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding = 0) uniform sampler2D samp; // for shader/hardware to visit OpenGL texture object, binding = 0 to bind samp to texture unit 0.

void main(void)
{
    color = texture(samp, tc); // color vary with position
}