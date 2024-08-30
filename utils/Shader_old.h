#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

#include "../glad/glad.h"
#include "../GLFW/glfw3.h"
#include "../stb/stb_image.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#ifdef _WIN32
    #define u_char unsigned char
#endif

class Shader {
public:
    GLuint programID;
    const char *vertexShaderSource;
    const char *fragmentShaderSource;
    float visibility = 0.2;
    float deltaTime;
    float lastFrame;

    struct ObjectInfo {
        GLuint VBO;
        GLuint VAO;
        GLuint EBO;
        bool init = false;
        struct TextureInfo {
            GLuint texture;
            GLuint ID;
            int width, height, nrChannels;
            bool init = false;
        };
        std::map<std::string, TextureInfo*> texmap;
        void newTexture(std::string texname);
        void newTexture(std::string texname, GLint WRAP_S, GLint WRAP_T, GLint MIN_FILTER, GLint MAG_FILTER, GLint internalformat, int flag_true_if_should_flip);
        TextureInfo* getTexture(std::string texname, int index);
    };


    Shader(const char *vertexShaderSrc, const char* fragmentShaderSrc);
    void use();
    void set1i(const char *name, GLint v0);
    void set1f(const char *name, GLfloat v0);
    void set3f(const char *name, GLfloat v0, GLfloat v1, GLfloat v2);
    void set3fv(const char *name, GLsizei count, GLfloat *value);
    void set4f(const char *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void set4fv(const char *name, GLsizei count, GLfloat *value);
    void setMatrix3fv(const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setMatrix4fv(const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
    std::multimap<std::string, std::unique_ptr<ObjectInfo>> objmap;
    void newObject(std::string objname);
    ObjectInfo* getObject(std::string objname, int index);
    float GetDeltaTime() {
        float curTime = (float)glfwGetTime();
        deltaTime = curTime - lastFrame;
        lastFrame = curTime;
        return deltaTime;
    }
    ~Shader() {}
};

std::unique_ptr<Shader> shader;
std::unique_ptr<Shader> lshader;
std::unique_ptr<Shader> tshader;
std::unique_ptr<Shader> cshader;
std::unique_ptr<Shader> cshader_smooth;
std::unique_ptr<Shader> cshader_sdf;

Shader::Shader(const char *vertexShaderSrcPath, const char *fragmentShaderSrcPath) {
    auto compileShaderSource = [](const char *filename, GLenum type, GLsizei count) -> GLuint {
        FILE* fp = fopen(filename, "r");
        if (!fp) {
            std::cerr << "Failed to open file" << std::endl;
        }
        char line[512];
        std::string result;
        while (fgets(line, sizeof(line), fp)) {
            result += std::string(line);
        }
        fclose(fp);
        const char* shaderSource = result.c_str();
        // std::cout << shaderSource << std::endl;

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
    GLuint vertexShader = compileShaderSource(vertexShaderSrcPath, GL_VERTEX_SHADER, 1);
    GLuint fragmentShader = compileShaderSource(fragmentShaderSrcPath, GL_FRAGMENT_SHADER, 1);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);
    // link异常处理
    int success;
    char logInfo[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, logInfo);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << logInfo << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(programID);
}

void Shader::set1i(const char *name, GLint v0) {
    glUniform1i(glGetUniformLocation(programID, name), v0);
}

void Shader::set1f(const char *name, GLfloat v0) {
    glUniform1f(glGetUniformLocation(programID, name), v0);
}

void Shader::set3f(const char *name, GLfloat v0, GLfloat v1, GLfloat v2) {
    glUniform3f(glGetUniformLocation(programID, name), v0, v1, v2);
}

void Shader::set3fv(const char *name, GLsizei count, GLfloat *value) {
    glUniform3fv(glGetUniformLocation(programID, name), count, value);
}

void Shader::set4f(const char *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform4f(glGetUniformLocation(programID, name), v0, v1, v2, v3);
}

void Shader::set4fv(const char *name, GLsizei count, GLfloat *value) {
    glUniform4fv(glGetUniformLocation(programID, name), count, value);
}

void Shader::setMatrix3fv(const char *name, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix3fv(glGetUniformLocation(programID, name), count, transpose, value);
}

void Shader::setMatrix4fv(const char *name, GLsizei count, GLboolean transpose, const GLfloat *value) {
    glUniformMatrix4fv(glGetUniformLocation(programID, name), count, transpose, value);
}

void Shader::newObject(std::string objname) {
    objmap.insert({objname, std::make_unique<ObjectInfo>()});
}

Shader::ObjectInfo* Shader::getObject(std::string objname, int index = 0) {
    auto it = objmap.find(objname);
    std::advance(it, index);
    return (it != objmap.end()) ? it->second.get() : nullptr;
}

void Shader::ObjectInfo::newTexture(std::string texname, GLint WRAP_S, GLint WRAP_T, GLint MIN_FILTER, GLint MAG_FILTER, GLint internalformat, int flag_true_if_should_flip) {
    auto tex = new TextureInfo();
    // 生成纹理
    glGenTextures(1, &tex->texture);
    glBindTexture(GL_TEXTURE_2D, tex->texture);
    // 设置环绕方式 过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIN_FILTER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAG_FILTER);
    // 生成图片数据
    stbi_set_flip_vertically_on_load(flag_true_if_should_flip);
    u_char *data = stbi_load(texname.c_str(), &tex->width, &tex->height, &tex->nrChannels, 0);
    // 载入纹理 (纹理目标, 多级渐远纹理级别, 纹理储存格式, 宽, 高, 0, 源图像格式, 图像数据)
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, internalformat, tex->width, tex->height, 0, internalformat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); // 绑定多级纹理级别
    } else {
        std::cerr << "Gen Texture Data Error" << std::endl;
    }
    stbi_image_free(data);
    texmap.insert({texname, tex});
}

void Shader::ObjectInfo::newTexture(std::string texname) {
    auto tex = new TextureInfo();
    texmap.insert({texname, tex});
}

Shader::ObjectInfo::TextureInfo* Shader::ObjectInfo::getTexture(std::string texname, int index = 0) {
    auto it = texmap.find(texname);
    std::advance(it, index);
    return (it != texmap.end()) ? it->second : nullptr;
}

#endif