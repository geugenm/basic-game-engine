#version 330 core

const vec2 triangle_vertices[3] = vec2[](
vec2(-0.9, 0.1),
vec2(0.9, -0.7),
vec2(0.0, 0.9)
);

out vec2 vTexCoord;

void main() {
    gl_Position = vec4(triangle_vertices[gl_VertexID], 0.0, 1.0);
    vTexCoord = (gl_Position.xy + vec2(1.0, 1.0)) / 2.0;
}
