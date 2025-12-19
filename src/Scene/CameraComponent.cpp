#include "CameraComponent.h"
#include "Entity.h"
#include "Transform.h"

void CameraComponent::UpdateCameraFromTransform() {
    if (!m_entity) return;

    auto transform = m_entity->GetTransform();
    
    // 同步位置
    m_camera.Pos = transform->GetPosition();
    
    // 同步旋转（从四元数转换为欧拉角）
    glm::vec3 euler = transform->GetEulerAngles();
    m_camera.Yaw = euler.y;
    m_camera.Pitch = euler.x;
    
    // Camera的向量会在ProcessKeyboard等方法中自动更新
    // 或者我们手动计算Front向量
    glm::vec3 front;
    front.x = cos(glm::radians(m_camera.Pitch)) * cos(glm::radians(m_camera.Yaw));
    front.y = sin(glm::radians(m_camera.Pitch));
    front.z = cos(glm::radians(m_camera.Pitch)) * sin(glm::radians(m_camera.Yaw));
    m_camera.Front = glm::normalize(front);
    m_camera.Right = glm::normalize(glm::cross(m_camera.Front, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_camera.Up = glm::normalize(glm::cross(m_camera.Right, m_camera.Front));
}
