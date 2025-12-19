#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "demo/TestFilter1.h"
#include <iostream>

// 静态实例指针
Application* Application::s_instance = nullptr;

// GLFW回调函数
static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (Application::GetInstance()) {
        Application::GetInstance()->SetWindowSize(width, height);
    }
}

static void mouse_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (Application::GetInstance()) {
        Application::GetInstance()->ProcessMouseMovement(xpos, ypos);
    }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    if (Application::GetInstance()) {
        Application::GetInstance()->ProcessMouseScroll(yoffset);
    }
}

Application::Application(int width, int height, const char* title)
    : m_window(nullptr)
    , m_camera(glm::vec3(0.0f, 1.0f, 5.0f))
    , m_screenWidth(width)
    , m_screenHeight(height)
    , m_title(title)
    , m_firstMouse(true)
    , m_lastX(width / 2.0f)
    , m_lastY(height / 2.0f)
    , m_deltaTime(0.0f)
    , m_lastFrame(0.0f)
{
    s_instance = this;
}

Application::~Application() {
    ShutdownImGui();
    if (m_window) {
        glfwDestroyWindow(m_window);
    }
    glfwTerminate();
    s_instance = nullptr;
}

bool Application::Init() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 创建窗口
    m_window = glfwCreateWindow(m_screenWidth, m_screenHeight, m_title, nullptr, nullptr);
    if (!m_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // 设置视口
    glViewport(0, 0, m_screenWidth, m_screenHeight);

    // 注册回调
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetCursorPosCallback(m_window, mouse_pos_callback);
    glfwSetScrollCallback(m_window, scroll_callback);

    // 初始化ImGui
    InitImGui();

    return true;
}

void Application::InitImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
}

void Application::ShutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Application::SetWindowSize(int width, int height) {
    m_screenWidth = width;
    m_screenHeight = height;
    glViewport(0, 0, width, height);
}

void Application::ProcessMouseMovement(double xpos, double ypos) {
    if (m_firstMouse) {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouse = false;
    }

    float xOffset = xpos - m_lastX;
    float yOffset = m_lastY - ypos;
    m_lastX = xpos;
    m_lastY = ypos;

    // 更新ImGui鼠标位置
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(xpos, ypos);

    m_camera.ProcessMouseMovement(xOffset, yOffset);
}

void Application::ProcessMouseScroll(double yoffset) {
    m_camera.ProcessMouseScrool(yoffset);
}

void Application::ProcessInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(m_window, true);
    }
    
    m_camera.ProcessKeyboard(m_window, m_deltaTime);
}

void Application::RenderImGui() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    
    ImGui::Begin("Debug");
    ImGui::Checkbox("Update MouseMove", &m_camera.UpdateMouseMv);
    ImGui::Checkbox("Update Keyboard", &m_camera.UpdateKeyboard);
    ImGui::ColorEdit4("Clear Color", (float*)&clear_color);
    int screen[2] = {m_screenWidth, m_screenHeight};
    ImGui::InputInt2("Screen", screen);
    ImGui::SliderFloat("Pitch", &m_camera.Pitch, -89.0f, 89.0f);
    ImGui::SliderFloat("Yaw", &m_camera.Yaw, -180.0f, 180.0f);
    ImGui::SliderFloat("Camera Zoom", &m_camera.Zoom, 1.0f, 45.0f);
    ImGui::End();

    ImGui::Render();
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, 
                 clear_color.z * clear_color.w, clear_color.w);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::RenderFrame() {
    // 清除缓冲
    m_renderer.Clear();
    m_renderer.EnableDepthTest();

    // 渲染场景
    if (m_activeScene) {
        m_activeScene->Render(m_screenWidth, m_screenHeight);
    }
}

void Application::Run() {
    while (!glfwWindowShouldClose(m_window)) {
        // 计算帧时间
        float currentFrame = glfwGetTime();
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;

        // 处理事件
        glfwPollEvents();
        ProcessInput();

        // 更新场景
        if (m_activeScene) {
            m_activeScene->Update(m_deltaTime);
        }

        // 渲染
        RenderFrame();
        
        // 渲染ImGui
        RenderImGui();

        // 交换缓冲
        glfwSwapBuffers(m_window);
    }
}
