#include "Transform.h"
#include "Entity.h"

const glm::mat4& Transform::GetWorldMatrix() {
    if (m_worldMatrixDirty) {
        // 获取父节点的世界矩阵
        if (m_entity) {
            auto parent = m_entity->GetParent();
            if (parent) {
                auto parentTransform = parent->GetTransform();
                m_worldMatrix = parentTransform->GetWorldMatrix() * GetLocalMatrix();
            } else {
                m_worldMatrix = GetLocalMatrix();
            }
        } else {
            m_worldMatrix = GetLocalMatrix();
        }
        m_worldMatrixDirty = false;
    }
    return m_worldMatrix;
}
