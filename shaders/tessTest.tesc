#version 460 core
layout (vertices=3) out;

void main()
{
    if (gl_InvocationID == 0)
    {
        gl_TessLevelOuter[0] = 4;
        gl_TessLevelOuter[1] = 4;
        gl_TessLevelOuter[2] = 4;

        gl_TessLevelInner[0] = 4;
    }
}