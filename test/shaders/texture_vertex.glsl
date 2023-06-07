#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform;

// border size in texture coordinates
const float border_size = 0.05;

void main()
{
    // calculate texture coordinates with border
    vec2 tex_coord_with_border = texCoord * (1.0 - border_size * 2.0) + border_size;

    // output color to show border
    if (texCoord.x < border_size || texCoord.y < border_size ||
    texCoord.x > 1.0 - border_size || texCoord.y > 1.0 - border_size) {
        ourColor = vec3(1.0, 1.0, 1.0);
    } else {
        ourColor = color;
    }

    gl_Position = transform * vec4(position, 1.0f);
    TexCoord = tex_coord_with_border;
}
