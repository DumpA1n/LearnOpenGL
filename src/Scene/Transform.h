#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

// Transform组件：管理位置、旋转、缩放
class Transform : public Component {
public:
    Transform() 
        : m_position(0.0f)
        , m_rotation(1.0f, 0.0f, 0.0f, 0.0f) // 单位四元数
        , m_scale(1.0f)
        , m_localMatrixDirty(true)
        , m_worldMatrixDirty(true)
    {}

    std::string GetTypeName() const override { return "Transform"; }

    // 位置
    void SetPosition(const glm::vec3& pos) {
        m_position = pos;
        SetDirty();
    }
    const glm::vec3& GetPosition() const { return m_position; }

    // 旋转（四元数）
    void SetRotation(const glm::quat& rot) {
        m_rotation = rot;
        SetDirty();
    }
    const glm::quat& GetRotation() const { return m_rotation; }

    // 旋转（欧拉角，单位：度）
    void SetEulerAngles(const glm::vec3& euler) {
        m_rotation = glm::quat(glm::radians(euler));
        SetDirty();
    }
    glm::vec3 GetEulerAngles() const {
        return glm::degrees(glm::eulerAngles(m_rotation));
    }

    // 缩放
    void SetScale(const glm::vec3& scale) {
        m_scale = scale;
        SetDirty();
    }
    const glm::vec3& GetScale() const { return m_scale; }

    // 方向向量
    glm::vec3 GetForward() const {
        return m_rotation * glm::vec3(0.0f, 0.0f, -1.0f);
    }
    glm::vec3 GetUp() const {
        return m_rotation * glm::vec3(0.0f, 1.0f, 0.0f);
    }
    glm::vec3 GetRight() const {
        return m_rotation * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    // 局部变换矩阵
    const glm::mat4& GetLocalMatrix() {
        if (m_localMatrixDirty) {
            UpdateLocalMatrix();
        }
        return m_localMatrix;
    }

    // 世界变换矩阵（考虑父节点）
    const glm::mat4& GetWorldMatrix();

    // 标记为脏（需要重新计算）
    void SetDirty() {
        m_localMatrixDirty = true;
        m_worldMatrixDirty = true;
    }

private:
    void UpdateLocalMatrix() {
        m_localMatrix = glm::mat4(1.0f);
        m_localMatrix = glm::translate(m_localMatrix, m_position);
        m_localMatrix *= glm::mat4_cast(m_rotation);
        m_localMatrix = glm::scale(m_localMatrix, m_scale);
        m_localMatrixDirty = false;
    }

    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::vec3 m_scale;
    
    glm::mat4 m_localMatrix;
    glm::mat4 m_worldMatrix;
    bool m_localMatrixDirty;
    bool m_worldMatrixDirty;
};
