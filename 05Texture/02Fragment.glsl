#version 430

in vec2 tc; // texture coordinates input, the interpolation result from rasterizer
out vec4 color;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int doMipmapping;
layout (binding = 0) uniform sampler2D samp; // for shader/hardware to visit OpenGL texture object, binding = 0 to bind samp to texture unit 0.
layout (binding = 1) uniform sampler2D mipmappingSamp;

void main(void)
{
    if (doMipmapping == 1)
    {
        color = texture(mipmappingSamp, tc); // color vary with position
    }
    else
    {
        color = texture(samp, tc); // color vary with position
    }
}