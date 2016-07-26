#version 150

in vec3 Color;

out vec4 outColor;

in uniform sampler2D;

void main()
{
    outColor = vec4(Color, 1.0);
}
