#version 150

in vec2 texcoord;
in vec4 finalColor;

uniform sampler2D tex0;

out vec4 outColor;

void main()
{
    outColor = texture(tex0, texcoord) * finalColor;
}

