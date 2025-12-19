#pragma once

#include "Component.h"
#include "../Resources/Shader.h"
#include "../Resources/Geometry.h"
#include "../Resources/Texture.h"
#include <memory>
#include <glm/glm.hpp>

// MeshRenderer组件：渲染网格
class MeshRenderer : public Component {
public:
    MeshRenderer() 
        : m_shader(nullptr)
        , m_geometry(nullptr)
        , m_texture(nullptr)
        , m_color(1.0f)
    {}

    std::string GetTypeName() const override { return "MeshRenderer"; }

    // 设置材质
    void SetShader(std::shared_ptr<Shader> shader) { m_shader = shader; }
    std::shared_ptr<Shader> GetShader() const { return m_shader; }

    // 设置几何体
    void SetGeometry(std::shared_ptr<Geometry> geometry) { m_geometry = geometry; }
    std::shared_ptr<Geometry> GetGeometry() const { return m_geometry; }

    // 设置纹理
    void SetTexture(std::shared_ptr<Texture> texture) { m_texture = texture; }
    std::shared_ptr<Texture> GetTexture() const { return m_texture; }

    // 颜色
    void SetColor(const glm::vec4& color) { m_color = color; }
    const glm::vec4& GetColor() const { return m_color; }

    // 渲染
    void Render(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);

private:
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<Geometry> m_geometry;
    std::shared_ptr<Texture> m_texture;
    glm::vec4 m_color;
};
