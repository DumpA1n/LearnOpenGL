#pragma once

#include "MTFilterManager.h"

class TestFilter1 : public MTFilter {
public:
    TestFilter1() {
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
    ~TestFilter1() = default;

    static void render_demo() {
        auto filter = MTFilterManager::instance()->getFilter("TestFilter1");

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
};

REGISTER_FILTER_CLASS("TestFilter1", TestFilter1)
