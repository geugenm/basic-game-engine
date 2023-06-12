#version 330 core

in vec2 vTexCoord;
out vec4 outColor;

uniform float time;

float noise(vec2 x) {
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);
    float n = p.x + p.y * 57.0;
    return mix(mix(fract(sin(n) * 753.5453123), fract(sin(n + 1.0) * 753.5453123), f.x),
               mix(fract(sin(n + 57.0) * 753.5453123), fract(sin(n + 58.0) * 753.5453123), f.x), f.y);
}

void main() {
    vec2 p = vTexCoord * 6.0;
    float n = 0.0;
    n += noise(p * 1.0 + vec2(0.0, time * 0.1)) * 1.0;
    n += noise(p * 2.0 + vec2(0.0, time * 0.2)) * 0.5;
    n += noise(p * 4.0 + vec2(0.0, time * 0.4)) * 0.25;
    n = pow(n, 3.0);

    // Add some color variation
    vec3 col = vec3(n, 0.3, 0.7);
    col = mix(col, vec3(1.0, 0.5, 0.0), 0.2);
    col = mix(col, vec3(0.0, 0.3, 1.0), 0.3);

    // Add some glow effect
    float glow = pow(n, 10.0);
    col += vec3(0.2, 0.2, 1.0) * glow;
    col = mix(col, vec3(1.0, 1.0, 1.0), 0.2);

    // Add some distortion effect
    float distortion = noise(p * 20.0 + vec2(0.0, time * 0.5)) * 0.3;
    p.x += distortion * (vTexCoord.x - 0.5) * 0.5;
    p.y += distortion * (vTexCoord.y - 0.5) * 0.5;
    col = mix(col, vec3(1.0, 0.0, 1.0), 0.1);

    // Apply final color and output
    outColor = vec4(col, 1.0);
}
