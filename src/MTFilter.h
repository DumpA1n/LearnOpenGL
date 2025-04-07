#pragma once

#include "src/Shader.h"

class MTFilter {
public:
    std::unordered_map<std::string, Shader*> ShaderList;
    std::unordered_map<std::string, FrameBuffer*> FrameBufferList;
    std::unordered_map<std::string, Texture*> TextureList;

    MTFilter() {}
    virtual ~MTFilter() {
        for (auto& it : ShaderList) { delete it.second; }
        for (auto& it : FrameBufferList) { delete it.second; }
        for (auto& it : TextureList) { delete it.second; }
    }

    Shader* newShader(std::string name, const char* vert, const char* frag) {
        Shader* shader = new Shader(vert, frag);
        ShaderList[name] = shader;
        return shader;
    }
    Shader* getShader(std::string name) {
        return ShaderList[name];
    }

    FrameBuffer* newFrameBuffer(std::string name) {
        FrameBuffer* framebuffer = new FrameBuffer{name};
        FrameBufferList[name] = framebuffer;
        return framebuffer;
    }
    FrameBuffer* getFrameBuffer(std::string name) {
        return FrameBufferList[name];
    }

    Texture* newTexture(std::string name, const char* filename) {
        Texture* texture = new Texture{filename};
        TextureList[name] = texture;
        return texture;
    }
    Texture* getTexture(std::string name) {
        return TextureList[name];
    }
};


class Test1 : public MTFilter {
public:
    Test1() {
        newShader("shader", "../res/shader/test.vs", "../res/shader/test.fs");
        newShader("fba", "../res/shader/fba.vs", "../res/shader/fba.fs");
        newFrameBuffer("fba");
        newFrameBuffer("fbb");
        newFrameBuffer("fbc");
        newTexture("box", "../res/container.jpg");
        getTexture("box")->setTexParament(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        newTexture("face", "../res/awesomeface.png");
        getTexture("face")->setTexParament(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
    }
    ~Test1() {}
};

void render_demo() {
    static std::unique_ptr<Test1> filter = std::make_unique<Test1>();

    Shader* _shader = filter->getShader("shader");
    Shader* _shader_fba = filter->getShader("fba");
    FrameBuffer* _fba = filter->getFrameBuffer("fba");
    FrameBuffer* _fbb = filter->getFrameBuffer("fbb");
    FrameBuffer* _fbc = filter->getFrameBuffer("fbc");
    Texture* _box  = filter->getTexture("box");
    Texture* _face = filter->getTexture("face");

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
