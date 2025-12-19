#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "../Utils/Camera.h"
#include "Renderer.h"
#include "../Scene/Scene.h"
#include <memory>

class Application {
public:
    Application(int width = 1200, int height = 800, const char* title = "Renderer");
    ~Application();

    // 禁止拷贝和赋值
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    
    // 获取全局实例
    static Application* GetInstance() { return s_instance; }

    // 初始化
    bool Init();
    
    // 运行主循环
    void Run();
    
    // 获取窗口
    GLFWwindow* GetWindow() const { return m_window; }
    
    // 获取相机
    Camera& GetCamera() { return m_camera; }
    const Camera& GetCamera() const { return m_camera; }
    
    // 获取渲染器
    Renderer& GetRenderer() { return m_renderer; }
    const Renderer& GetRenderer() const { return m_renderer; }
    
    // 获取窗口尺寸
    int GetWidth() const { return m_screenWidth; }
    int GetHeight() const { return m_screenHeight; }
    
    // 场景管理
    void SetActiveScene(std::shared_ptr<Scene> scene) { m_activeScene = scene; }
    std::shared_ptr<Scene> GetActiveScene() const { return m_activeScene; }
    
    // 设置窗口尺寸（由回调函数调用）
    void SetWindowSize(int width, int height);
    
    // 处理鼠标移动
    void ProcessMouseMovement(double xpos, double ypos);
    
    // 处理滚轮
    void ProcessMouseScroll(double yoffset);

private:
    void ProcessInput();
    void InitImGui();
    void ShutdownImGui();
    void RenderFrame();
    void RenderImGui();

private:
    static Application* s_instance;
    
    GLFWwindow* m_window;
    Camera m_camera;
    Renderer m_renderer;
    std::shared_ptr<Scene> m_activeScene;
    int m_screenWidth;
    int m_screenHeight;
    const char* m_title;
    
    // 鼠标状态
    bool m_firstMouse;
    float m_lastX;
    float m_lastY;
    
    // 时间
    float m_deltaTime;
    float m_lastFrame;
};
