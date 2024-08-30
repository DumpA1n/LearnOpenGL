#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texBox;
uniform sampler2D texFace;

void main()
{
    vec4 color = mix(texture(texBox, TexCoord), texture(texFace, TexCoord), 0.2);
    FragColor = color;
}
