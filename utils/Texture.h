#pragma once

#include <iostream>
#include <cstdio>
#ifdef ____MACOS____
    #include <unistd.h>
#endif
#include <string>
#include <map>
#include <unordered_map>
#include <memory>

#include "stb/stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Texture {
public:
    uint32_t id;
    int width;
    int height;
    int nrChannels;
    Texture() {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        setTexParament(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, id, 0);
    }
    Texture(const char* filename) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        setTexParament(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
        unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);
        uint32_t internalformat = GL_RGBA;
        if (data) {
            // (纹理目标, 多级渐远纹理级别, 纹理储存格式, 宽, 高, 0, 源图像格式, 图像数据)
            if (strstr(filename, ".png")) {
                internalformat = GL_RGBA;
            } else if (strstr(filename, ".jpg") || strstr(filename, ".jpeg")) {
                internalformat = GL_RGB;
            }
            glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, internalformat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Gen Texture Data Error" << std::endl;
        }
        stbi_image_free(data);
    }
    ~Texture() {}
    void setTexParament(uint32_t WRAP_S = GL_CLAMP_TO_EDGE, uint32_t WRAP_T = GL_CLAMP_TO_EDGE, uint32_t MIN_FILTER = GL_LINEAR_MIPMAP_LINEAR, uint32_t MAG_FILTER = GL_LINEAR) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WRAP_S);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WRAP_T);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MIN_FILTER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MAG_FILTER);
    }
};
