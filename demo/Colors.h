#pragma once
#ifndef COLORS_H
#define COLORS_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "Draw3D.h"
#include "RenderText.h"

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void DrawLight(GLFWwindow* window) {
    if (!lshader) {
        lshader = std::make_unique<Shader>("light_shader.vs", "light_shader.fs");
    }
    if (lshader->objmap.find("light") == lshader->objmap.end()) {
        lshader->newObject("light");
    }
    Shader::ObjectInfo* light = lshader->getObject("light");
    if (!light->init) {
        glGenVertexArrays(1, &light->VAO);
        glBindVertexArray(light->VAO);
        glGenBuffers(1, &light->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, light->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glGenBuffers(1, &light->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, light->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(1);

        light->newTexture("awesomeface.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, GL_RGBA, true);
        lshader->use();
        lshader->set1i("texture0", 0);
    
        light->init = true;
    }
    lshader->use();
    glBindVertexArray(light->VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, light->getTexture("awesomeface.png")->texture);

    camera.ProcessKeyboard(window, lshader->GetDeltaTime());

    glm::mat4 model = glm::mat4(1.0f);
              model = glm::translate(model, lightPos);
              model = glm::scale(model, glm::vec3(0.2f));
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    lshader->setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
    lshader->setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
    lshader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    ImVec2 screenPos = WorldToScreen(lightPos, glm::mat4(1.0f), view, projection);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screenWidth / 2.0f, 0), screenPos, IM_COL32(255, 255, 0, 255), 1.0f);
    char lightText[24];
    snprintf(lightText, sizeof(lightText), "Light(ImGui API)");
    ImVec2 textSize = ImGui::CalcTextSize(lightText);
    // ImGui::GetBackgroundDrawList()->AddText(ImVec2(screenPos.x - textSize.x / 2, screenPos.y), IM_COL32(22, 255, 235, 255), lightText);
    // ImGui::RenderBullet(ImGui::GetBackgroundDrawList(), ImVec2(screenPos.x, screenPos.y), IM_COL32(22, 255, 235, 255));
    RenderText("Light", glm::vec2(screenPos.x, screenPos.y), glm::vec4(0.9f, 0.5f, 0.5f, 1.0f), 1.0f, true);
    // RenderText("Test Text 180 ABO Normal", glm::vec2(screenPos.x, screenPos.y), glm::vec4(1.0f), 3.0f);
    // RenderText("Test Text 180 ABO Smooth", glm::vec2(screenPos.x, screenPos.y), glm::vec4(1.0f), 3.0f, false);
    // RenderText("Test Text 180 ABO SDF", glm::vec2(screenPos.x, screenPos.y), glm::vec4(1.0f), 3.0f, true);
}

#endif