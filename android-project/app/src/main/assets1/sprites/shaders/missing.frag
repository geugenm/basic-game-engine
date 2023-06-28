#version 300 es
precision mediump float; // Specify medium precision for floats

out vec4 color;

void main()
{
    const vec4 red = vec4(1.0, 0.0, 0.0, 1.0);

    color = red;
}

