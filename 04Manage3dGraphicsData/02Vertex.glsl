#version 430

layout (location = 0) in vec3 position; // the vertex array buffer.
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform float tf;

out vec4 varyingColor;

// forward declaration
mat4 buildTranslate(float x, float y, float z);
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buldScale(float x, float y, float z);

void main(void)
{
    // x,y,z for translation
    float i = gl_InstanceID + tf;
    float a = sin(203.0 * i / 8000.0) * 401.0;
    float b = cos(301.0 * i / 4001.0) * 402.0;
    float c = sin(400.0 * i / 6003.0) * 405.0;
    
    // rotation and translation matrices
    mat4 localRotX = buildRotateX(1.75 * i);
    mat4 localRotY = buildRotateY(1.75 * i);
    mat4 localRotZ = buildRotateZ(1.75 * i);
    mat4 localTrans = buildTranslate(a, b, c);

    // build model matrix, then model-view matrix
    mat4 newM_matrix = m_matrix * localTrans * localRotX * localRotY * localRotZ;
    mat4 mv_matrix = v_matrix * newM_matrix;

    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

// build translation matrix, matrix in GLSL is column-first
mat4 buildTranslate(float x, float y, float z)
{
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0, // this is first column
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                        x,   y,   z, 1.0);
    return trans;
}
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
// scale matrix
mat4 buldScale(float x, float y, float z)
{
    mat4 scale = mat4(  x, 0.0, 0.0, 0.0,
                      0.0,   y, 0.0, 0.0,
                      0.0, 0.0, z,   0.0,
                      0.0, 0.0, 0.0, 1.0);
    return scale;
}