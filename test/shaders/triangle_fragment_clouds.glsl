#version 330 core

in vec2 vTexCoord;
out vec4 outColor;

uniform float time;

float hash(vec2 p) {
    float h = dot(p, vec2(127.1, 311.7));
    return fract(sin(h) * 43758.5453123);
}

float noise(vec2 x) {
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0;
    float a = hash(p + vec2(0.0, 0.0));
    float b = hash(p + vec2(1.0, 0.0));
    float c = hash(p + vec2(0.0, 1.0));
    float d = hash(p + vec2(1.0, 1.0));
    float u = f.x * f.x * (3.0 - 2.0 * f.x);
    float v = f.y * f.y * (3.0 - 2.0 * f.y);
    return mix(mix(a, b, u), mix(c, d, u), v);
}

void main() {
    vec2 p = vTexCoord * 6.0;
    float n = 0.0;
    n += noise(p * 1.0 + vec2(0.0, time * 0.1)) * 0.5;
    n += noise(p * 2.0 + vec2(0.0, time * 0.2)) * 0.25;
    n += noise(p * 4.0 + vec2(0.0, time * 0.4)) * 0.125;
    n += noise(p * 8.0 + vec2(0.0, time * 0.8)) * 0.0625;
    n += noise(p * 16.0 + vec2(0.0, time * 1.6)) * 0.03125;
    n += noise(p * 32.0 + vec2(0.0, time * 3.2)) * 0.015625;
    n = pow(n, 3.0);
    vec3 col = vec3(0.3, 0.6, 1.0) * n;
    outColor = vec4(col, 1.0);
}
