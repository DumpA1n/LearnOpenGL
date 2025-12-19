#pragma once

#include "Component.h"
#include "Transform.h"
#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>

class Scene;

// Entity：场景中的对象，管理组件集合
class Entity : public std::enable_shared_from_this<Entity> {
public:
    Entity(const std::string& name = "Entity");
    ~Entity();

    // 名称
    const std::string& GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    // 启用/禁用
    void SetActive(bool active) { m_active = active; }
    bool IsActive() const { return m_active; }

    // Transform（每个Entity必有）
    Transform* GetTransform() { return m_transform.get(); }

    // 组件管理
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
        
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        component->SetEntity(this);
        
        std::type_index typeIndex(typeid(T));
        m_components[typeIndex] = component;
        
        component->OnCreate();
        return component.get();
    }

    template<typename T>
    T* GetComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
        
        std::type_index typeIndex(typeid(T));
        auto it = m_components.find(typeIndex);
        if (it != m_components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T>
    bool HasComponent() {
        return GetComponent<T>() != nullptr;
    }

    template<typename T>
    void RemoveComponent() {
        std::type_index typeIndex(typeid(T));
        auto it = m_components.find(typeIndex);
        if (it != m_components.end()) {
            it->second->OnDestroy();
            m_components.erase(it);
        }
    }

    // 层级关系
    void SetParent(std::shared_ptr<Entity> parent);
    std::shared_ptr<Entity> GetParent() const { return m_parent.lock(); }
    const std::vector<std::shared_ptr<Entity>>& GetChildren() const { return m_children; }
    
    void AddChild(std::shared_ptr<Entity> child);
    void RemoveChild(std::shared_ptr<Entity> child);

    // 更新
    void Update(float deltaTime);

    // 获取所属场景
    Scene* GetScene() const { return m_scene; }
    void SetScene(Scene* scene) { m_scene = scene; }

private:
    std::string m_name;
    bool m_active;
    
    std::shared_ptr<Transform> m_transform;
    std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
    
    std::weak_ptr<Entity> m_parent;
    std::vector<std::shared_ptr<Entity>> m_children;
    
    Scene* m_scene;
};
