#pragma once

#include "glad/glad.h"
#include <glm/glm.hpp>

// 渲染器核心类 - 管理OpenGL状态和渲染操作
class Renderer {
public:
    Renderer();
    ~Renderer();
    
    // 禁止拷贝
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    // 清屏操作
    void Clear(const glm::vec4& color = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
    void ClearColor();
    void ClearDepth();
    
    // OpenGL状态管理
    void EnableDepthTest();
    void DisableDepthTest();
    void EnableBlend();
    void DisableBlend();
    void EnableCullFace();
    void DisableCullFace();
    void EnableWireframe();
    void DisableWireframe();
    
    // 设置清屏颜色
    void SetClearColor(const glm::vec4& color);
    void SetClearColor(float r, float g, float b, float a = 1.0f);
    
    // 视口管理
    void SetViewport(int x, int y, int width, int height);
    void SetViewport(int width, int height);
    
    // 混合模式
    void SetBlendMode(GLenum sfactor, GLenum dfactor);
    void SetBlendModeAlpha();
    void SetBlendModeAdditive();
    
    // 深度测试
    void SetDepthFunc(GLenum func);
    
    // 面剔除
    void SetCullFace(GLenum mode);
    
    // 获取状态
    bool IsDepthTestEnabled() const { return m_depthTestEnabled; }
    bool IsBlendEnabled() const { return m_blendEnabled; }
    bool IsCullFaceEnabled() const { return m_cullFaceEnabled; }
    
    // 获取统计信息
    void ResetStats();
    int GetDrawCalls() const { return m_drawCalls; }
    int GetTriangles() const { return m_triangles; }
    
    // 记录绘制调用（供内部使用）
    void RecordDrawCall(int triangleCount = 0);

private:
    glm::vec4 m_clearColor;
    bool m_depthTestEnabled;
    bool m_blendEnabled;
    bool m_cullFaceEnabled;
    
    // 统计信息
    int m_drawCalls;
    int m_triangles;
};
