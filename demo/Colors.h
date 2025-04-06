#pragma once

#include <vector>

#include "include/glad/glad.h"
#include "include/GLFW/glfw3.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "utils/Shader.h"
#include "utils/Camera.h"
#include "utils/Texture.h"
#include "RenderText.h"

#include "utils/Rectangle.h"

extern int screenWidth, screenHeight;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void DrawLight(GLFWwindow* window) {
    static std::unique_ptr<Shader> shader = std::make_unique<Shader>("../res/shader/light_shader.vs", "../res/shader/light_shader.fs");;
    static std::shared_ptr<Rectangle> light = std::make_shared<Rectangle>();

    shader->use();

    glBindVertexArray(light->VAO);

    camera.ProcessKeyboard(window, shader->getDeltaTime());

    glm::mat4 model = glm::mat4(1.0f);
              model = glm::translate(model, lightPos);
              model = glm::scale(model, glm::vec3(0.2f));
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader->setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
    shader->setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
    shader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    ImVec2 screenPos = WorldToScreen(lightPos, glm::mat4(1.0f), view, projection);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screenWidth / 2.0f, 0), screenPos, IM_COL32(255, 255, 0, 255), 1.0f);
    char lightText[24];
    snprintf(lightText, sizeof(lightText), "Light(ImGui API)");
    ImVec2 textSize = ImGui::CalcTextSize(lightText);
    ImGui::GetBackgroundDrawList()->AddText(ImVec2(screenPos.x - textSize.x / 2, screenPos.y), IM_COL32(22, 255, 235, 255), lightText);
    // ImGui::RenderBullet(ImGui::GetBackgroundDrawList(), ImVec2(screenPos.x, screenPos.y), IM_COL32(22, 255, 235, 255));
    // RenderText("Light", glm::vec2(screenPos.x, screenPos.y), glm::vec4(0.9f, 0.5f, 0.5f, 1.0f), 1.0f, true);
    // RenderText("Test Text 180 ABO Normal", glm::vec2(screenPos.x, screenPos.y), glm::vec4(1.0f), 3.0f);
    // RenderText("Test Text 180 ABO Smooth", glm::vec2(screenPos.x, screenPos.y), glm::vec4(1.0f), 3.0f, false);
    // RenderText("Test Text 180 ABO SDF", glm::vec2(screenPos.x, screenPos.y), glm::vec4(1.0f), 3.0f, true);
}
