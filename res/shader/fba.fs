#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texBox;
uniform sampler2D texFace;

void main()
{
    vec4 color = texture(texFace, TexCoord);
    FragColor = color;
    // FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // FragColor = vec4(vec3(1.0 - texture(texFace, TexCoord)), 1.0);
}
