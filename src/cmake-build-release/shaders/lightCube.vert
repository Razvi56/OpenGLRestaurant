#version 410 core

layout(location=0) in vec3 vPosition;
layout(location=1) in vec3 vNormal;
layout(location=2) in vec2 vTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out DATA
{
	mat4 projection;
} data_out;

void main() 
{
	gl_Position = view * model * vec4(vPosition, 1.0f);
	data_out.projection = projection;
}
