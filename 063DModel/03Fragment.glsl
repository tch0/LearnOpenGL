#version 430

in vec4 varyingColor;

in vec2 tc;
out vec4 color;
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding = 0) uniform sampler2D samp;

void main(void)
{
    // color = varyingColor; // color vary with position
    color = texture(samp, tc);
}