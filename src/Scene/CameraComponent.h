#pragma once

#include "Component.h"
#include "../Utils/Camera.h"
#include <glm/glm.hpp>

// CameraComponent：相机组件
class CameraComponent : public Component {
public:
    CameraComponent(const glm::vec3& position = glm::vec3(0.0f, 0.0f, 3.0f))
        : m_camera(position)
        , m_isMainCamera(false)
        , m_fieldOfView(45.0f)
        , m_nearPlane(0.1f)
        , m_farPlane(100.0f)
    {}

    std::string GetTypeName() const override { return "CameraComponent"; }

    void OnCreate() override {
        // 同步Transform到Camera
        UpdateCameraFromTransform();
    }

    void OnUpdate(float deltaTime) override {
        UpdateCameraFromTransform();
    }

    // 相机访问
    Camera& GetCamera() { return m_camera; }
    const Camera& GetCamera() const { return m_camera; }

    // 主相机标记
    void SetMainCamera(bool isMain) { m_isMainCamera = isMain; }
    bool IsMainCamera() const { return m_isMainCamera; }

    // 投影参数
    void SetFieldOfView(float fov) { m_fieldOfView = fov; }
    float GetFieldOfView() const { return m_fieldOfView; }

    void SetNearPlane(float near) { m_nearPlane = near; }
    float GetNearPlane() const { return m_nearPlane; }

    void SetFarPlane(float far) { m_farPlane = far; }
    float GetFarPlane() const { return m_farPlane; }

    // 获取视图矩阵
    glm::mat4 GetViewMatrix() {
        return m_camera.GetViewMatrix();
    }
    // 获取投影矩阵
    glm::mat4 GetProjectionMatrix(float aspect) const {
        return glm::perspective(glm::radians(m_fieldOfView), aspect, m_nearPlane, m_farPlane);
    }

private:
    void UpdateCameraFromTransform();

    Camera m_camera;
    bool m_isMainCamera;
    
    float m_fieldOfView;
    float m_nearPlane;
    float m_farPlane;
};
