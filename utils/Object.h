#pragma once

#include <string>
#include <map>

#include "glad/glad.h"

class Object {
public:
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    Object() {}
    virtual ~Object() {}
    virtual void gen() = 0;
};