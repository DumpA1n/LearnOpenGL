#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texBox;
uniform sampler2D texFace;

void main()
{
    vec4 color = texture(texFace, TexCoord);
    FragColor = color;
}
