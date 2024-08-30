#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aNormal;

out vec4 vertexColor;
out vec4 setColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform float offset = 0.0f;
uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    Normal = normalMatrix * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

    // gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    // vertexColor = aColor; // 因 片段插值 和 坐标<=0，左下角为黑色
    // vertexColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}