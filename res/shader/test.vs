#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform vec2 u_Flip;
uniform vec2 screeSize;
uniform vec2 texBoxSize;
uniform vec2 texFaceSize;

void main() {
    gl_Position = vec4(aPosition, 1.0);
    gl_Position.xy *= u_Flip;
    TexCoord = aTexCoord;
    vec2 radio = vec2(1.0);
}