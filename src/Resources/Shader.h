#pragma once

#include <string>
#include <memory>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Texture;
class Geometry;

class Shader {
public:
    GLuint program;
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
    void draw(const Geometry* geometry) const;
    void draw(const std::shared_ptr<Geometry>& geometry) const;
    float getDeltaTime();

private:
    GLuint compileShader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource);
};
