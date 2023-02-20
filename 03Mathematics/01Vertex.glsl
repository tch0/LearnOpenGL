#version 430
uniform float angle;
// rotate around the x-axis
mat4 buildRotateX(float rad)
{
    mat4 xrot = mat4(1.0,      0.0,      0.0,  0.0,
                     0.0, cos(rad), -sin(rad), 0.0,
                     0.0, sin(rad),  cos(rad), 0.0,
                     0.0,      0.0,       0.0, 1.0);
    return xrot;
}
// rotate around the y-axis
mat4 buildRotateY(float rad)
{
    mat4 yrot = mat4( cos(rad), 0.0, sin(rad), 0.0,
                           0.0, 1.0,      0.0, 0.0,
                     -sin(rad), 0.0, cos(rad), 0.0,
                           0.0, 0.0,      0.0, 1.0);
    return yrot;
}
// rotate around the z-axis
mat4 buildRotateZ(float rad)
{
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                     sin(rad),  cos(rad), 0.0, 0.0,
                          0.0,       0.0, 1.0, 0.0,
                          0.0,       0.0, 0.0, 1.0);
    return zrot;
}
void main(void)
{
    // rotate the triangle around the Y-axis
    mat4 mat = buildRotateY(angle);
    if (gl_VertexID == 0)
    {
        gl_Position = mat * vec4(0.25, -0.25, 0.0, 1.0);
    }
    else if (gl_VertexID == 1)
    {
        gl_Position = mat * vec4(-0.25, -0.25, 0.0, 1.0);
    }
    else
    {
        gl_Position = mat * vec4(0.25, 0.25, 0.0, 1.0);
    }
}