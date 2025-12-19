#pragma once

#include "Entity.h"
#include "CameraComponent.h"
#include "MeshRenderer.h"
#include <string>
#include <vector>
#include <memory>

// Scene：场景管理器
class Scene {
public:
    Scene(const std::string& name = "Scene");
    ~Scene();

    // 场景名称
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    // Entity管理
    std::shared_ptr<Entity> CreateEntity(const std::string& name = "Entity");
    void AddEntity(std::shared_ptr<Entity> entity);
    void RemoveEntity(std::shared_ptr<Entity> entity);
    const std::vector<std::shared_ptr<Entity>>& GetRootEntities() const { return m_rootEntities; }

    // 查找Entity
    std::shared_ptr<Entity> FindEntityByName(const std::string& name);

    // 相机管理
    void SetMainCamera(CameraComponent* camera);
    CameraComponent* GetMainCamera() const { return m_mainCamera; }

    // 场景更新
    void Update(float deltaTime);

    // 场景渲染
    void Render(int screenWidth, int screenHeight);

private:
    void RenderEntity(std::shared_ptr<Entity> entity, const glm::mat4& view, const glm::mat4& projection);

    std::string m_name;
    std::vector<std::shared_ptr<Entity>> m_rootEntities;
    CameraComponent* m_mainCamera;
};
