#pragma once

#include <glad/glad.h>

class GLBuffer {
public:
    GLBuffer(GLenum type) : bufferType_(type) {
        glGenBuffers(1, &bufferId_);
    }

    ~GLBuffer() {
        glDeleteBuffers(1, &bufferId_);
    }

    void bind() {
        glBindBuffer(bufferType_, bufferId_);
    }

    void unbind() {
        glBindBuffer(bufferType_, 0);
    }

private:
    GLuint bufferId_;
    GLenum bufferType_;
};
