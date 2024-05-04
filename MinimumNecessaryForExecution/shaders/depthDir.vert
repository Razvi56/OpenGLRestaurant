#version 410 core
layout(location=0) in vec3 vPosition;
uniform mat4 lightSpaceTrMatrixDir;
uniform mat4 model;
void main()
{
    gl_Position = lightSpaceTrMatrixDir * model * vec4(vPosition, 1.0f);
}