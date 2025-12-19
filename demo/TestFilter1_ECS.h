#pragma once

#include "MTFilterManager.h"
#include "src/Core/Application.h"
#include "src/Scene/Scene.h"
#include "src/Scene/Entity.h"
#include "src/Scene/MeshRenderer.h"
#include "src/Scene/CameraComponent.h"
#include "glm/detail/type_mat.hpp"
#include "glm/detail/type_vec.hpp"

class TestFilter1_ECS : public MTFilter {
public:
    TestFilter1_ECS() {
        newShader("shader", "../res/shader/test.vs", "../res/shader/test.fs");
        newShader("fba", "../res/shader/fba.vs", "../res/shader/fba.fs");
        newFrameBuffer("fba");
        newFrameBuffer("fbb");
        newFrameBuffer("fbc");
        newTexture("box", "../res/container.jpg");
        getTexture("box")->setTexParament(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        newTexture("face", "../res/awesomeface.png");
        getTexture("face")->setTexParament(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);
        
        // 创建几何体
        auto geom = newGeometry("quad");
        
        // 顶点数据（全屏quad）
        float vertices[] = {
             1.0f,  1.0f, 0.0f, 1.0f,  1.0f,
             1.0f, -1.0f, 0.0f, 1.0f,  0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,  0.0f,
            -1.0f,  1.0f, 0.0f, 0.0f,  1.0f
        };
        
        // 索引数据
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };
        
        geom->setVertexData(vertices, sizeof(vertices), GL_DYNAMIC_DRAW);
        geom->setIndexData(indices, 6);
        geom->setStandardLayout();
    }
    ~TestFilter1_ECS() = default;

    // 使用Scene系统创建场景
    static std::shared_ptr<Scene> CreateScene() {
        auto scene = std::make_shared<Scene>("TestScene");
        
        auto filter = MTFilterManager::instance()->getFilter("TestFilter1");
        auto _shader = filter->getShader("shader");
        auto _box = filter->getTexture("box");
        auto _geom = filter->getGeometry("quad");
        
        // 创建相机实体
        auto cameraEntity = scene->CreateEntity("MainCamera");
        cameraEntity->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        auto cameraComp = cameraEntity->AddComponent<CameraComponent>();
        scene->SetMainCamera(cameraComp);
        
        // 创建渲染对象
        auto quadEntity = scene->CreateEntity("Quad");
        quadEntity->GetTransform()->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        quadEntity->GetTransform()->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
        
        auto renderer = quadEntity->AddComponent<MeshRenderer>();
        renderer->SetShader(_shader);
        renderer->SetGeometry(_geom);
        renderer->SetTexture(_box);
        
        return scene;
    }

    static void render_demo() {
        auto filter = MTFilterManager::instance()->getFilter("TestFilter1");

        auto _shader = filter->getShader("shader");
        auto _shader_fba = filter->getShader("fba");
        auto _fba = filter->getFrameBuffer("fba");
        auto _box  = filter->getTexture("box");
        auto _face = filter->getTexture("face");
        auto _geom = filter->getGeometry("quad");

        float vertices[] = {
            0.5f,  0.5f, 0.0f, 1.0f,  1.0f, 
            0.5f, -0.5f, 0.0f, 1.0f,  0.0f, 
            -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, 
            -0.5f,  0.5f, 0.0f, 0.0f,  1.0f  
        };
        
        glm::vec2 u_Flip = glm::vec2(1.0, -1.0);
        auto app = Application::GetInstance();
        glm::vec2 screeSize = glm::vec2(app ? app->GetWidth() : 1200, app ? app->GetHeight() : 800);
        glm::vec2 texBoxSize = glm::vec2(_box->width, _box->height);
        glm::vec2 texFaceSize = glm::vec2(_face->width, _face->height);

        _fba->bind();
        _shader_fba->use();
        _shader_fba->set2fv("screeSize", glm::value_ptr(screeSize));
        _shader_fba->set2fv("texFaceSize", glm::value_ptr(texFaceSize));
        _shader_fba->setSampler("texFace", _face.get());
        _shader_fba->draw(_geom);

        _shader->use();
        _shader->set2fv("u_Flip", glm::value_ptr(u_Flip));
        _shader->set2fv("screeSize", glm::value_ptr(screeSize));
        _shader->set2fv("texBoxSize", glm::value_ptr(texBoxSize));
        _shader->setSampler("texBox", _box.get());
        _shader->setSampler("texFace", _fba->getSampler());
        
        // 更新顶点数据
        _geom->updateVertexData(vertices, sizeof(vertices));
        _shader->draw(_geom);
    }
};

REGISTER_FILTER_CLASS("TestFilter1_ECS", TestFilter1_ECS)
