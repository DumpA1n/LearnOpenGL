#pragma once

#include "utils/Texture.h"

class FrameBuffer {
public:
    uint32_t FBO;
    uint32_t RBO;
    Texture* texture;
    int width;
    int height;
    FrameBuffer(std::string name) {
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        texture = new Texture();

        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    Texture* getSampler() {
        return texture;
    }
    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    }
    ~FrameBuffer();
};
