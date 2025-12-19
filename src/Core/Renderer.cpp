#include "Renderer.h"

Renderer::Renderer()
    : m_clearColor(0.2f, 0.3f, 0.3f, 1.0f)
    , m_depthTestEnabled(false)
    , m_blendEnabled(false)
    , m_cullFaceEnabled(false)
    , m_drawCalls(0)
    , m_triangles(0)
{
    // 不在构造函数中调用OpenGL函数
    // 等待OpenGL context创建后再初始化
}

Renderer::~Renderer() {
}

void Renderer::Clear(const glm::vec4& color) {
    SetClearColor(color);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearColor() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepth() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::EnableDepthTest() {
    if (!m_depthTestEnabled) {
        glEnable(GL_DEPTH_TEST);
        m_depthTestEnabled = true;
    }
}

void Renderer::DisableDepthTest() {
    if (m_depthTestEnabled) {
        glDisable(GL_DEPTH_TEST);
        m_depthTestEnabled = false;
    }
}

void Renderer::EnableBlend() {
    if (!m_blendEnabled) {
        glEnable(GL_BLEND);
        m_blendEnabled = true;
    }
}

void Renderer::DisableBlend() {
    if (m_blendEnabled) {
        glDisable(GL_BLEND);
        m_blendEnabled = false;
    }
}

void Renderer::EnableCullFace() {
    if (!m_cullFaceEnabled) {
        glEnable(GL_CULL_FACE);
        m_cullFaceEnabled = true;
    }
}

void Renderer::DisableCullFace() {
    if (m_cullFaceEnabled) {
        glDisable(GL_CULL_FACE);
        m_cullFaceEnabled = false;
    }
}

void Renderer::EnableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Renderer::DisableWireframe() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::SetClearColor(const glm::vec4& color) {
    m_clearColor = color;
    glClearColor(color.r, color.g, color.b, color.a);
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    SetClearColor(glm::vec4(r, g, b, a));
}

void Renderer::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::SetViewport(int width, int height) {
    glViewport(0, 0, width, height);
}

void Renderer::SetBlendMode(GLenum sfactor, GLenum dfactor) {
    glBlendFunc(sfactor, dfactor);
}

void Renderer::SetBlendModeAlpha() {
    SetBlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::SetBlendModeAdditive() {
    SetBlendMode(GL_ONE, GL_ONE);
}

void Renderer::SetDepthFunc(GLenum func) {
    glDepthFunc(func);
}

void Renderer::SetCullFace(GLenum mode) {
    glCullFace(mode);
}

void Renderer::ResetStats() {
    m_drawCalls = 0;
    m_triangles = 0;
}

void Renderer::RecordDrawCall(int triangleCount) {
    m_drawCalls++;
    m_triangles += triangleCount;
}
