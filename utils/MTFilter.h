#pragma once
#ifndef MTFILTER_H
#define MTFILTER_H

#include "utils/Shader.h"

class MTFilter {
public:

    bool IsInit = false;

    std::unordered_map<std::string, Shader*> ShaderList;
    std::unordered_map<std::string, FrameBuffer*> FrameBufferList;
    std::unordered_map<std::string, Texture*> TextureList;

    Shader* newShader(std::string name, const char* vert, const char* frag) {
        Shader* shader = new Shader(vert, frag);
        ShaderList[name] = shader;
        return shader;
    }
    Shader* getShader(std::string name) {
        return ShaderList[name];
    }

    FrameBuffer* newFrameBuffer(std::string name) {
        FrameBuffer* framebuffer = new FrameBuffer(name);
        FrameBufferList[name] = framebuffer;
        framebuffer->texture->index = TextureList.size();
        TextureList[name] = framebuffer->texture;
        return framebuffer;
    }
    FrameBuffer* getFrameBuffer(std::string name) {
        return FrameBufferList[name];
    }

    Texture* newTexture(std::string name, const char* filename) {
        Texture* texture = new Texture(filename);
        texture->index = TextureList.size();
        TextureList[name] = texture;
        return texture;
    }
    Texture* getTexture(std::string name) {
        return TextureList[name];
    }

} MTFilter;



void Initialize() {
    MTFilter.newShader("shader", "../res/shader/test.vs", "../res/shader/test.fs");
    MTFilter.newShader("fba", "../res/shader/fba.vs", "../res/shader/fba.fs");
    MTFilter.newFrameBuffer("fba");
    MTFilter.newFrameBuffer("fbb");
    MTFilter.newFrameBuffer("fbc");
    MTFilter.newTexture("box", "container.jpg");
    MTFilter.getTexture("box")->setTexParament(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    MTFilter.newTexture("face", "awesomeface.png");
    MTFilter.getTexture("face")->setTexParament(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    MTFilter.IsInit = true;
}

void render_demo() {
    if (!MTFilter.IsInit) Initialize();

    Shader* _shader = MTFilter.getShader("shader");
    Shader* _shader_fba = MTFilter.getShader("fba");
    FrameBuffer* _fba = MTFilter.getFrameBuffer("fba");
    FrameBuffer* _fbb = MTFilter.getFrameBuffer("fbb");
    FrameBuffer* _fbc = MTFilter.getFrameBuffer("fbc");
    Texture* _box  = MTFilter.getTexture("box");
    Texture* _face = MTFilter.getTexture("face");

    float vertices[] = {
         0.5f,  0.5f, 0.0f, 1.0f,  1.0f, // 右上
         0.5f, -0.5f, 0.0f, 1.0f,  0.0f, // 右下
        -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, // 左下
        -0.5f,  0.5f, 0.0f, 0.0f,  1.0f  // 左上
    };
    glm::vec2 u_Flip = glm::vec2(1.0, -1.0);
    glm::vec2 screeSize = glm::vec2(screenWidth, screenHeight);
    glm::vec2 texBoxSize = glm::vec2(_box->width, _box->height);
    glm::vec2 texFaceSize = glm::vec2(_face->width, _face->height);

    _fba->bind();
    _shader_fba->use();
    _shader_fba->set2fv("screeSize", glm::value_ptr(screeSize));
    _shader_fba->set2fv("texFaceSize", glm::value_ptr(texFaceSize));
    _shader_fba->setSampler("texFace", _face);
    _shader_fba->setVertexData();
    _shader_fba->draw();

    _shader->use();
    _shader->set2fv("u_Flip", glm::value_ptr(u_Flip));
    _shader->set2fv("screeSize", glm::value_ptr(screeSize));
    _shader->set2fv("texBoxSize", glm::value_ptr(texBoxSize));
    _shader->setSampler("texBox", _box);
    _shader->setSampler("texFace", _fba->getSampler());
    _shader->setVertexData(vertices);
    _shader->draw();
}

#endif // MTFILTER_H