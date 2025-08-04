#pragma once

#include "src/Shader.h"
#include "MTFilterManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class MTFilter {
public:
    MTFilter() = default;
    virtual ~MTFilter() {
        for (auto& it : m_ShaderList) { delete it.second; }
        for (auto& it : m_FrameBufferList) { delete it.second; }
        for (auto& it : m_TextureList) { delete it.second; }
    }

    Shader* newShader(std::string name, const char* vert, const char* frag) {
        Shader* shader = new Shader(vert, frag);
        m_ShaderList[name] = shader;
        return shader;
    }
    Shader* getShader(std::string name) {
        return m_ShaderList[name];
    }

    FrameBuffer* newFrameBuffer(std::string name) {
        FrameBuffer* framebuffer = new FrameBuffer{name};
        m_FrameBufferList[name] = framebuffer;
        return framebuffer;
    }
    FrameBuffer* getFrameBuffer(std::string name) {
        return m_FrameBufferList[name];
    }

    Texture* newTexture(std::string name, const char* filename) {
        Texture* texture = new Texture{filename};
        m_TextureList[name] = texture;
        return texture;
    }
    Texture* getTexture(std::string name) {
        return m_TextureList[name];
    }

private:
    std::unordered_map<std::string, Shader*> m_ShaderList;
    std::unordered_map<std::string, FrameBuffer*> m_FrameBufferList;
    std::unordered_map<std::string, Texture*> m_TextureList;
};
