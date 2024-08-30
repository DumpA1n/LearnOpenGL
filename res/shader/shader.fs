#version 330 core
out vec4 FragColor;

in vec4 vertexColor;
in vec4 ourColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float visibility;
uniform vec4 setColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float speculerStrength = 0.5;
    vec3 cameraDir = normalize(cameraPos - FragPos);
    
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);

    // FragColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.2 + visibility) * vec4(result, 1.0);
    // FragColor = ourColor;
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}