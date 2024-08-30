#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D text;
uniform vec4 textColor;
uniform float time;

void main()
{
    float dist = texture(text, TexCoord).r;

    if (dist < 0.5)
        discard;

    vec3 col = vec3(1.0);
    if (dist >= 0.5 && dist < 0.55) {
        float red = 0.5 + 0.5 * sin(time);
        float green = 0.5 + 0.5 * sin(time + 2);
        float blue = 0.5 + 0.5 * sin(time + 4);
        col = vec3(red, green, blue);
    }

    dist = smoothstep(0.4, 0.6, dist);
    FragColor = vec4(col, dist + 0.23);
    // FragColor = vec4(textColor.rgb, dist + 0.23);
}