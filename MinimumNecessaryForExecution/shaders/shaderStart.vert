#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;


out vec3 fNormal;
out vec4 fPosWorld;
out vec2 fTexCoords;
out vec4 fragPosLightSpace;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform mat4 lSDTM;


void main()
{
    //compute eye space coordinates
    fPosWorld = model * vec4(vPosition, 1.0f);///fragment position in the coordinate systems with origin at camera position
    fNormal = normalize(normalMatrix * vNormal);///normalized normal in eye space
    fTexCoords = vTexCoords;
    fragPosLightSpace = lSDTM * model * vec4(vPosition, 1.0f);
    gl_Position = projection * view * model * vec4(vPosition, 1.0f);///position of vertex, sent to the rest of the pipeline
}
