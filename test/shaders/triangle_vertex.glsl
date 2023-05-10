#version 330 core

const vec2 triangle_vertices[3] = vec2[](
vec2(2.5, 0.1),
vec2(0.5, 0.96),
vec2(0.1, 0.9)
);

void main()
{
    vec2 vertexPosition = triangle_vertices[gl_VertexID];
    vec2 vertexColor = vec2(gl_VertexID * 0.5 + 0.5);

    gl_Position = vec4(vertexPosition, 0.0, 1.0);
    gl_PointSize = 10.0; // Set the point size for a posh appearance
}
