#include "MeshRenderer.h"
#include "Entity.h"
#include "Transform.h"
#include <glm/gtc/type_ptr.hpp>

void MeshRenderer::Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix) {
    if (!m_shader || !m_geometry || !IsEnabled()) {
        return;
    }

    // 获取变换矩阵
    auto transform = m_entity->GetTransform();
    glm::mat4 modelMatrix = transform->GetWorldMatrix();

    // 使用着色器
    m_shader->use();

    // 设置MVP矩阵
    m_shader->setMatrix4fv("model", glm::value_ptr(modelMatrix));
    m_shader->setMatrix4fv("view", glm::value_ptr(viewMatrix));
    m_shader->setMatrix4fv("projection", glm::value_ptr(projectionMatrix));

    // 设置颜色
    m_shader->set4fv("color", glm::value_ptr(m_color));

    // 绑定纹理
    if (m_texture) {
        m_shader->setSampler("texture1", m_texture.get());
    }

    // 绘制
    m_shader->draw(m_geometry);
}
