#version 300 es

precision mediump float; // Specify medium precision for floats

in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture1;

void main()
{
    color = texture(texture1, TexCoord);
}
