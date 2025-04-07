#pragma once

#include "src/FrameBuffer.h"

class Shader {
public:
    GLuint program;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    uint32_t TextureUnit;
    float lastFrame;
    float visibility = 0.2;

    Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
    ~Shader() {}
    void use();
    void draw();
    void drawElements(GLenum mode, GLsizei count, GLenum type, const void *indices);
    void drawArray(GLenum mode, GLint first, GLsizei count);
    void set1i(const char *name, GLint v0);
    void set1f(const char *name, GLfloat v0);
    void set2f(const char *name, GLfloat v0, GLfloat v1);
    void set2fv(const char *name, GLsizei count, GLfloat *value);
    void set2fv(const char *name, GLfloat *value);
    void set3f(const char *name, GLfloat v0, GLfloat v1, GLfloat v2);
    void set3fv(const char *name, GLsizei count, GLfloat *value);
    void set3fv(const char *name, GLfloat *value);
    void set4f(const char *name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void set4fv(const char *name, GLsizei count, GLfloat *value);
    void set4fv(const char *name, GLfloat *value);
    void setMatrix3fv(const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setMatrix3fv(const char *name, const GLfloat *value);
    void setMatrix4fv(const char *name, GLsizei count, GLboolean transpose, const GLfloat *value);
    void setMatrix4fv(const char *name, const GLfloat *value);
    void setSampler(const char *name, const Texture* texture);
    void setSampler(const char *name, GLuint texId);
    void setVertexData(float *vertices, size_t size = 80);
    void setVertexData();
    float getDeltaTime();
};
