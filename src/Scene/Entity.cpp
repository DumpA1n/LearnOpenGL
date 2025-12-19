#include "Entity.h"
#include <algorithm>

Entity::Entity(const std::string& name)
    : m_name(name)
    , m_active(true)
    , m_scene(nullptr)
{
    // 每个Entity都必须有Transform
    m_transform = std::make_shared<Transform>();
    m_transform->SetEntity(this);
}

Entity::~Entity() {
    // 销毁所有组件
    for (auto& pair : m_components) {
        pair.second->OnDestroy();
    }
    m_components.clear();
}

void Entity::SetParent(std::shared_ptr<Entity> parent) {
    // 从旧父节点移除
    if (auto oldParent = m_parent.lock()) {
        oldParent->RemoveChild(shared_from_this());
    }
    
    // 设置新父节点
    m_parent = parent;
    if (parent) {
        parent->AddChild(shared_from_this());
    }
    
    // 标记Transform为脏
    m_transform->SetDirty();
}

void Entity::AddChild(std::shared_ptr<Entity> child) {
    if (!child) return;
    
    // 避免重复添加
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it == m_children.end()) {
        m_children.push_back(child);
    }
}

void Entity::RemoveChild(std::shared_ptr<Entity> child) {
    if (!child) return;
    
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        m_children.erase(it);
    }
}

void Entity::Update(float deltaTime) {
    if (!m_active) return;
    
    // 更新所有组件
    for (auto& pair : m_components) {
        if (pair.second->IsEnabled()) {
            pair.second->OnUpdate(deltaTime);
        }
    }
    
    // 更新子节点
    for (auto& child : m_children) {
        child->Update(deltaTime);
    }
}
