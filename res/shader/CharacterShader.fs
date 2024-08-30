#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D text;
uniform vec4 textColor;
uniform float time;

void main()
{
    float alpha = texture(text, TexCoord).r;

    vec3 col = vec3(1.0);
    if (alpha >= 0.5 && alpha < 0.9) {
        float red = 0.5 + 0.5 * sin(time);
        float green = 0.5 + 0.5 * sin(time + 2);
        float blue = 0.5 + 0.5 * sin(time + 4);
        col = vec3(red, green, blue);
    }

    FragColor = vec4(col, alpha);
}
