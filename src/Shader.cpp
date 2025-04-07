#include "Shader.h"

Shader::Shader(const char *vertexShaderSource, const char *fragmentShaderSource) {
    auto compileShaderSource = [](const char *filename, GLenum type, GLsizei count) -> GLuint {
        std::string _shaderSource;
        if (strstr(filename, ".vs") || strstr(filename, ".fs") || 
            strstr(filename, ".vert") || strstr(filename, ".frag")) {
            FILE* fp = fopen(filename, "r");
            if (!fp) {
                std::cerr << "Compile Shader Error: Failed to open file" << std::endl;
            }
            char line[512];
            std::string result;
            while (fgets(line, sizeof(line), fp)) {
                result += std::string(line);
            }
            fclose(fp);
            _shaderSource = result;
        } else {
            _shaderSource = std::string(filename);
        }
        const char* shaderSource = _shaderSource.c_str();

        GLuint glShader = glCreateShader(type); // 创建着色器对象
        glShaderSource(glShader, count, &shaderSource, NULL); // 创建着色器
        glCompileShader(glShader); // 编译
        // 异常处理
        GLint success;
        char logInfo[512];
        glGetShaderiv(glShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(glShader, sizeof(logInfo), NULL, logInfo);
            std::cerr << "ShaderSouce compile error: " << logInfo << std::endl;
        }
        return glShader;
    };
    GLuint vertexShader = compileShaderSource(vertexShaderSource, GL_VERTEX_SHADER, 1);
    GLuint fragmentShader = compileShaderSource(fragmentShaderSource, GL_FRAGMENT_SHADER, 1);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // link异常处理
    int success;
    char logInfo[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, logInfo);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << logInfo << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    uint32_t indices[] = {
        0, 1, 3,
        1, 2, 3
    };
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 5 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Shader::use() {
    TextureUnit = 0;
    glUseProgram(program);
}

void Shader::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shader::drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices) {
    glBindVertexArray(VAO);
    glDrawElements(mode, count, type, indices);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shader::drawArray(GLenum mode, GLint first, GLsizei count) {
    glBindVertexArray(VAO);
    glDrawArrays(mode, first, count);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Shader::set1i(const char *name, GLint v0) {
    glUniform1i(glGetUniformLocation(program, name), v0);
}
void Shader::set1f(const char *name, GLfloat v0) {
    glUniform1f(glGetUniformLocation(program, name), v0);
}

void Shader::set2f(const char *name, GLfloat v0, GLfloat v1) {
    glUniform2f(glGetUniformLocation(program, name), v0, v1);
}
void Shader::set2fv(const char *name, GLsizei count, GLfloat *value) {
    glUniform2fv(glGetUniformLocation(program, name), count, value);
}
void Shader::set2fv(const char *name, GLfloat *value) {
    glUniform2fv(glGetUniformLocation(program, name), 1, value);
}

void Shader::set3f(const char *name, GLfloat v0, GLfloat v1, GLfloat v2) {
    glUniform3f(glGetUniformLocation(program, name), v0, v1, v2);
}
void Shader::set3fv(const char *name, GLsizei count, GLfloat *value) {
    glUniform3fv(glGetUniformLocation(program, name), count, value);
}
void Shader::set3fv(const char *name, GLfloat *value) {
    glUniform3fv(glGetUniformLocation(program, name), 1, value);
}

void Shader::set4f(const char *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform4f(glGetUniformLocation(program, name), v0, v1, v2, v3);
}
void Shader::set4fv(const char *name, GLsizei count, GLfloat *value) {
    glUniform4fv(glGetUniformLocation(program, name), count, value);
}
void Shader::set4fv(const char *name, GLfloat *value) {
    glUniform4fv(glGetUniformLocation(program, name), 1, value);
}

void Shader::setMatrix3fv(const char *name, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix3fv(glGetUniformLocation(program, name), count, transpose, value);
}
void Shader::setMatrix3fv(const char *name, const GLfloat *value) {
    glUniformMatrix3fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}

void Shader::setMatrix4fv(const char *name, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), count, transpose, value);
}
void Shader::setMatrix4fv(const char *name, const GLfloat *value) {
    glUniformMatrix4fv(glGetUniformLocation(program, name), 1, GL_FALSE, value);
}

void Shader::setSampler(const char *name, const Texture* texture) {
    set1i(name, TextureUnit);
    glActiveTexture(GL_TEXTURE0 + TextureUnit);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    TextureUnit++;
}

void Shader::setSampler(const char *name, GLuint texId) {
    set1i(name, TextureUnit);
    glActiveTexture(GL_TEXTURE0 + TextureUnit);
    glBindTexture(GL_TEXTURE_2D, texId);
    TextureUnit++;
}

void Shader::setVertexData(float *vertices, size_t size) {
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

void Shader::setVertexData() {
    float vertices[] = {
         1.0f,  1.0f, 0.0f, 1.0f,  1.0f, 
         1.0f, -1.0f, 0.0f, 1.0f,  0.0f, 
        -1.0f, -1.0f, 0.0f, 0.0f,  0.0f, 
        -1.0f,  1.0f, 0.0f, 0.0f,  1.0f  
    };
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
}

float Shader::getDeltaTime() {
    float curTime = (float)glfwGetTime();
    float deltaTime = curTime - lastFrame;
    lastFrame = curTime;
    return deltaTime;
}
