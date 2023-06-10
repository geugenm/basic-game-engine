#version 330 core

in vec2 vTexCoord;
out vec4 outColor;

uniform float time;
uniform vec3 color;

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

    // Add fluid and cloud-like movement
    float t = time * 0.05;
    float x = p.x + (sin(p.y * 5.0 + t * 2.0) + sin(p.y * 10.0 + t * 1.5)) * 0.1;
    float y = p.y + (sin(p.x * 5.0 + t * 1.5) + sin(p.x * 10.0 + t * 2.0)) * 0.1;
    p = vec2(x, y);

    float n = 0.0;
    n += noise(p * 1.0 + vec2(0.0, time * 0.1)) * 1.0;
    n += noise(p * 2.0 + vec2(0.0, time * 0.2)) * 0.5;
    n += noise(p * 4.0 + vec2(0.0, time * 0.4)) * 0.25;

    // Add depth and volume effect
    n = pow(n, 4.0);
    vec3 col = vec3(n * 0.5 + 0.5);

    // Add color variation
    col = mix(col, vec3(0.3, 0.6, 1.0), 0.2);
    col = mix(col, vec3(1.0, 0.3, 0.7), 0.3);
    col = mix(col, color, 0.1);

    // Add glow effect
    float glow = pow(n, 10.0);
    col += vec3(1.0, 1.0, 1.0) * glow * 0.5;

    // Add distortion effect
    float distortion = noise(p * 20.0 + vec2(0.0, time * 0.5)) * 0.3;
    p.x += distortion * (vTexCoord.x - 0.5) * 0.5;
    p.y += distortion * (vTexCoord.y - 0.5) * 0.5;

    // Apply final color and output
    outColor = vec4(col, 1.0);
}
