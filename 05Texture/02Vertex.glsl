#version 430

layout (location = 0) in vec3 position; // the model vertex buffer
layout (location = 1) in vec2 texureCoord; // the texture vertex buffer
out vec2 tc; // texture coordinates output
uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int doMipmapping;
layout (binding = 0) uniform sampler2D samp; // for shader/hardware to visit OpenGL texture object, binding = 0 to bind samp to texture unit 0.
layout (binding = 1) uniform sampler2D mipmappingSamp;

void main(void)
{
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    tc = texureCoord;
}
