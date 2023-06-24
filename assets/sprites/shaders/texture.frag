#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture;

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

    vec4 bottomColor = texture2D(texture, TexCoord);


    // Frame animation 2.0 case
    vec2 normalized_texture = TexCoord;

    if (normalized_texture.x > 1.0) {
        normalized_texture.x = normalized_texture.x - 1.0;
    }

    if (normalized_texture.y > 1.0) {
        normalized_texture.y = normalized_texture.y - 1.0;
    }

    vec2 diagonal_start = vec2(border_thickness, border_thickness);
    vec2 diagonal_end = vec2(1.0 - border_thickness, 1.0 - border_thickness);

    bool near_border = normalized_texture.x < border_thickness || normalized_texture.x > 1.0 - border_thickness ||
    normalized_texture.y < border_thickness || normalized_texture.y > 1.0 - border_thickness;

    bool near_diagonal = near_line(normalized_texture, diagonal_start, diagonal_end, diagonal_thickness);


    if (near_border || near_diagonal) {
        color = light_purple;
    } else {
        color = bottomColor;
    }
}

