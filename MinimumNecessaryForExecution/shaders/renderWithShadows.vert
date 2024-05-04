#version 410 core

///vertex position in local space
layout(location=0) in vec3 vPosition;
///vertex normal in local space
layout(location=1) in vec3 vNormal;
///vertex texture mapping
layout(location=2) in vec2 vTexCoords;

///vertex normal sent to fragment shader, in world space
out vec3 fNormal;
///position sent to fragment shader, in eye space
out vec4 fPosEye;
///texture mapping sent to fragment shader
out vec2 fTexCoords;
///position of vertex in directional light space coordinates
out vec4 fragPosInDirLightSpace;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceDirTrMatrix;


void main()
{
    //compute eye space coordinates
    fPosEye = view * model * vec4(vPosition, 1.0f);///fragment position in the coordinate systems with origin at camera position
    fNormal = normalize(normalMatrix * vNormal);///normalized normal in eye space
    fTexCoords = vTexCoords;
    fragPosInDirLightSpace = lightSpaceDirTrMatrix * model * vec4(vPosition, 1.0f);
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);///position of vertex, sent to the rest of the pipeline
}