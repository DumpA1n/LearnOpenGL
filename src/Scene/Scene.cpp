#include "Scene.h"
#include <algorithm>

Scene::Scene(const std::string& name)
    : m_name(name)
    , m_mainCamera(nullptr)
{
}

Scene::~Scene() {
    m_rootEntities.clear();
}

std::shared_ptr<Entity> Scene::CreateEntity(const std::string& name) {
    auto entity = std::make_shared<Entity>(name);
    AddEntity(entity);
    return entity;
}

void Scene::AddEntity(std::shared_ptr<Entity> entity) {
    if (!entity) return;
    
    entity->SetScene(this);
    
    // 只添加根节点（没有父节点的实体）
    if (!entity->GetParent()) {
        m_rootEntities.push_back(entity);
    }
}

void Scene::RemoveEntity(std::shared_ptr<Entity> entity) {
    if (!entity) return;
    
    auto it = std::find(m_rootEntities.begin(), m_rootEntities.end(), entity);
    if (it != m_rootEntities.end()) {
        m_rootEntities.erase(it);
    }
    
    entity->SetScene(nullptr);
}

std::shared_ptr<Entity> Scene::FindEntityByName(const std::string& name) {
    std::function<std::shared_ptr<Entity>(const std::vector<std::shared_ptr<Entity>>&)> search;
    search = [&](const std::vector<std::shared_ptr<Entity>>& entities) -> std::shared_ptr<Entity> {
        for (auto& entity : entities) {
            if (entity->GetName() == name) {
                return entity;
            }
            // 递归搜索子节点
            auto found = search(entity->GetChildren());
            if (found) return found;
        }
        return nullptr;
    };
    
    return search(m_rootEntities);
}

void Scene::SetMainCamera(CameraComponent* camera) {
    m_mainCamera = camera;
    if (camera) {
        camera->SetMainCamera(true);
    }
}

void Scene::Update(float deltaTime) {
    // 更新所有根实体（会递归更新子节点）
    for (auto& entity : m_rootEntities) {
        entity->Update(deltaTime);
    }
}

void Scene::Render(int screenWidth, int screenHeight) {
    if (!m_mainCamera) {
        // 没有主相机，无法渲染
        return;
    }

    // 获取主相机的视图和投影矩阵
    float aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    glm::mat4 viewMatrix = m_mainCamera->GetViewMatrix();
    glm::mat4 projectionMatrix = m_mainCamera->GetProjectionMatrix(aspect);

    // 渲染所有实体
    for (auto& entity : m_rootEntities) {
        RenderEntity(entity, viewMatrix, projectionMatrix);
    }
}

void Scene::RenderEntity(std::shared_ptr<Entity> entity, const glm::mat4& view, const glm::mat4& projection) {
    if (!entity || !entity->IsActive()) {
        return;
    }

    // 渲染MeshRenderer组件
    auto meshRenderer = entity->GetComponent<MeshRenderer>();
    if (meshRenderer) {
        meshRenderer->Render(view, projection);
    }

    // 递归渲染子节点
    for (auto& child : entity->GetChildren()) {
        RenderEntity(child, view, projection);
    }
}
