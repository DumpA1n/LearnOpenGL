#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D text;
uniform vec4 textColor;
uniform float time;

void main()
{
    vec3 col = vec3(1.0);
    float alpha = texture(text, TexCoord).r;
    if (alpha > 0.0 && alpha < 0.9) {
        float red = 0.5 + 0.5 * sin(time);
        float green = 0.5 + 0.5 * sin(time + 2);
        float blue = 0.5 + 0.5 * sin(time + 4);
        col = vec3(red, green, blue);
    }
    alpha = smoothstep(0.4, 0.6, alpha);
    FragColor = vec4(col, alpha);
}


// #version 330 core

// in vec2 TexCoord;

// out vec4 FragColor;

// uniform sampler2D text;
// uniform vec4 textColor;

// void main()
// {
//     float alpha = texture(text, TexCoord).r;
//     alpha = smoothstep(0.4, 0.6, alpha);
//     FragColor = vec4(textColor.rgb, alpha);
// }
