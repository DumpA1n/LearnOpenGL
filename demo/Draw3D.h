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

#include "utils/Rectangle.h"

extern int screenWidth, screenHeight;

// extern glm::vec3 lightPos;

std::vector<glm::vec3> cubPos = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    // glm::vec3( 2.0f,  5.0f, -15.0f), 
    // glm::vec3(-1.5f, -2.2f, -2.5f),  
    // glm::vec3(-3.8f, -2.0f, -12.3f),  
    // glm::vec3( 2.4f, -0.4f, -3.5f),  
    // glm::vec3(-1.7f,  3.0f, -7.5f),  
    // glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3(-1.5f,  2.0f, -1.5f), 
    glm::vec3( 0.0f,  2.0f, -1.5f), 
    glm::vec3( 1.5f,  2.0f, -1.5f)
};

void Draw3DGraphic(GLFWwindow* window) {
    static std::unique_ptr<Shader> shader = std::make_unique<Shader>("../res/shader/shader.vs", "../res/shader/shader.fs");
    static std::shared_ptr<Rectangle> rectangle = std::make_shared<Rectangle>();
    static std::shared_ptr<Texture> container = std::make_shared<Texture>("../res/container.jpg");
    static std::shared_ptr<Texture> awesomeface = std::make_shared<Texture>("../res/awesomeface.png");

    shader->use();

    shader->setSampler("texture0", container.get());
    shader->setSampler("texture1", awesomeface.get());

    static glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    static glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    static glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
    shader->set3fv("lightPos", 1, glm::value_ptr(lightPos));
    shader->set3fv("lightColor", 1, glm::value_ptr(lightColor));
    shader->set3fv("objectColor", 1, glm::value_ptr(objectColor));

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        shader->visibility = (shader->visibility + 0.01f > 1.0f ? 1.0f : shader->visibility + 0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        shader->visibility = (shader->visibility - 0.01f < 0.0f ? 0.0f : shader->visibility - 0.01f);
    }
    shader->set1f("visibility", shader->visibility);

    glBindVertexArray(rectangle->VAO);

    // for (int i = 0; i < cubPos.size(); i++) {
    //     cubPos[i].y = 0.0f;
    // }

    camera.ProcessKeyboard(window, shader->getDeltaTime());

    glm::mat4 view = camera.GetViewMatrix();
    shader->setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

    for (int i = 0; i < cubPos.size(); i++) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubPos[i]);
        if (i == 1) {
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians((i + 1) * 20.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        } else if (i == 2) {
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians((i + 1) * 20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        } else if (i == 3) {
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians((i + 1) * 20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        }
        // 法向量
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        shader->setMatrix3fv("normalMatrix", 1, GL_FALSE, glm::value_ptr(normalMatrix));

        shader->setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        // shader->drawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        ImVec2 screenPos = WorldToScreen(cubPos[i], glm::mat4(1.0f), view, projection);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screenWidth / 2.0f, 0), screenPos, IM_COL32(255, 255, 0, 255), 1.0f);
        // ImGui::GetBackgroundDrawList()->AddCircle(screenPos, 10.0f, IM_COL32(255, 255, 0, 255), 0, 1.0f);
        static char cubText[16];
        snprintf(cubText, sizeof(cubText), "Cub%d", i);
        ImVec2 textSize = ImGui::CalcTextSize(cubText);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(screenPos.x - textSize.x / 2, screenPos.y), IM_COL32(22, 255, 235, 255), cubText);
    }
}
