#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture;

bool near_line(vec2 point, vec2 a, vec2 b, float thickness) {
    vec2 ab = b - a;
    vec2 ap = point - a;
    float t = dot(ap, ab) / dot(ab, ab);
    vec2 projection = mix(a, b, t);
    float distance = length(point - projection);
    return distance < thickness;
}


void main()
{
    const vec4 light_purple = vec4(0.7, 0.3, 0.8, 1.0);
    const float border_thickness = 0.02;
    const float diagonal_thickness = 0.005;

    vec2 diagonal_start = vec2(border_thickness, border_thickness);
    vec2 diagonal_end = vec2(1.0 - border_thickness, 1.0 - border_thickness);

    bool near_border = TexCoord.x < border_thickness || TexCoord.x > 1.0 - border_thickness ||
    TexCoord.y < border_thickness || TexCoord.y > 1.0 - border_thickness;

    bool near_diagonal = near_line(TexCoord, diagonal_start, diagonal_end, diagonal_thickness);

    vec4 bottomColor = texture2D(ourTexture, TexCoord);

    if (near_border || near_diagonal) {
        color = light_purple;
    } else {
        color = bottomColor;
    }
}

