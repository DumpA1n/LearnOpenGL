#include <iostream>
#include <stdio.h>

#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

int screenWidth = 1200;
int screenHeight = 800;

// #include "demo/DrawShape.h"
#include "demo/Draw3D.h"
#include "demo/Colors.h"
// #include "demo/RenderText.h"
#include "demo/Draw3D_Demo.h"
#include "utils/Camera.h"
#include "utils/MTFilter.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int getMaxnrAttributes() {
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    return nrAttributes;
}

bool isFirstMousemv = true;
float lastX = 400;
float lastY = 300;
void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    if (isFirstMousemv) {
        lastX = xpos;
        lastY = ypos;
        isFirstMousemv = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    // 更新 ImGui 的鼠标位置
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(xpos, ypos);

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void scrool_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScrool(yoffset);
}

bool text_debug_window = true;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // 多重采样
    // glfwWindowHint(GLFW_SAMPLES, 4);  // MSAA

    // 创建窗口对象
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL & ImGui", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // imgui Enable vsync

    // 初始化GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 初始化ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // io.Fonts->AddFontFromFileTTF("../res/fonts/Consolas.ttf", 15);
    // imgui style
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    // 渲染窗口尺寸大小
    glViewport(0, 0, screenWidth, screenHeight);

    // Set OpenGL options
    // glEnable(GL_CULL_FACE); // 易面剔除
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_MULTISAMPLE);

    // 注册回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetScrollCallback(window, scrool_callback);

    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // font_sdf.AddFontFromFileTTF("../res/fonts/Consolas.ttf", 0, 48, true);
    // font.AddFontFromFileTTF("../res/fonts/Consolas.ttf", 0, 48, false);

    float scale = 3.0f;

    // Rendering
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏鼠标指针
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Debug");
            ImGui::Checkbox("Update MouseMove", &camera.UpdateMouseMv);
            ImGui::Checkbox("Update Keyboard", &camera.UpdateKeyboard);
            ImGui::ColorEdit4("Clear Color", (float*)&clear_color);
            int screen[2] = {*(int*)&screenWidth, *(int*)&screenHeight};
            ImGui::InputInt2("Screen", screen);
            ImGui::SliderFloat("Pitch", &camera.Pitch, -89.0f, 89.0f);
            ImGui::SliderFloat("Yaw", &camera.Yaw, -180.0f, 180.0f);
            // ImGui::InputFloat3("Camere Pos", glm::value_ptr(camera.Pos));
            // ImGui::InputFloat3("Camere Front", glm::value_ptr(camera.Front));
            // ImGui::InputFloat3("Camere Right", glm::value_ptr(camera.Right));
            // ImGui::InputFloat3("Camere Up", glm::value_ptr(camera.Up));
            ImGui::SliderFloat("Camera Zoom", &camera.Zoom, 1.0f, 45.0f);
            ImGui::End();
        }

        // render_demo();
        // DrawCube();
        DrawLight(window);
        Draw3DGraphic(window);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



        glfwSwapBuffers(window);
    }

    // 清理ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}