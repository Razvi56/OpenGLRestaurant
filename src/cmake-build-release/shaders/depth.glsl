#version 410 core
layout(triangles) in; ///3 triangle vertices
layout (triangle_strip, max_vertices=18) out;///GS passes
uniform mat4 shadowCubeMatrices[6];

out vec4 fragPos;

void main() {
    for(int face = 0; face < 6; ++face)
    {
        gl_Layer = face; // built-in variable that specifies to which face we render.
        for(int i = 0; i < 3; ++i) // for each triangle vertex
        {
            fragPos = gl_in[i].gl_Position;
            gl_Position = shadowCubeMatrices[face] * fragPos;
            EmitVertex();
        }
        EndPrimitive();
    }
}
