#pragma once

#include <string>
#include <memory>

class Entity;

// 组件基类
class Component {
public:
    Component() : m_entity(nullptr), m_enabled(true) {}
    virtual ~Component() = default;

    // 生命周期方法
    virtual void OnCreate() {}
    virtual void OnUpdate(float deltaTime) {}
    virtual void OnDestroy() {}

    // 启用/禁用
    void SetEnabled(bool enabled) { m_enabled = enabled; }
    bool IsEnabled() const { return m_enabled; }

    // 获取所属Entity
    Entity* GetEntity() const { return m_entity; }
    void SetEntity(Entity* entity) { m_entity = entity; }

    // 组件类型名称（用于调试）
    virtual std::string GetTypeName() const = 0;

protected:
    Entity* m_entity;
    bool m_enabled;
};
