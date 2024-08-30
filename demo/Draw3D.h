#pragma once
#ifndef DRAW3D_H
#define DRAW3D_H

#include <vector>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Shader/Shader.h"
#include "Camera/Camera.h"

#ifdef _WIN32
    #define u_char unsigned char
#endif

extern int screenWidth, screenHeight;

extern glm::vec3 lightPos;

ImVec2 WorldToScreen(const glm::vec3& pos, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    glm::vec4 clipSpacePos = projection * view * model * glm::vec4(pos, 1.0);
    if (clipSpacePos.w > 0) {
        clipSpacePos /= clipSpacePos.w;
    }
    ImVec2 screenPos;
    screenPos.x = (clipSpacePos.x * 0.5f + 0.5f) * screenWidth;
    screenPos.y = (1.0f - clipSpacePos.y * 0.5f - 0.5f) * screenHeight;
    return screenPos;
}

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

float vertices[] = {
    //    顶点位置                  颜色                纹理坐标             法向量
    -0.5f, -0.5f,  0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,  0.0f,  1.0f, // 左下前 0
     0.5f, -0.5f,  0.5f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f,  0.0f,  1.0f, // 右下前 1
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f,  0.0f,  1.0f, // 右上前 2
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f,  0.0f,  1.0f, // 左上前 3

    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,  0.0f, -1.0f, // 左下后 4
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f,  0.0f, -1.0f, // 右下后 5
     0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f,  0.0f, -1.0f, // 右上后 6
    -0.5f,  0.5f, -0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f,  0.0f, -1.0f, // 左上后 7

    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // 左下后 8
    -0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,  -1.0f,  0.0f,  0.0f, // 左上后 9
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // 左上前 10
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,  -1.0f,  0.0f,  0.0f, // 左下前 11

     0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   1.0f,  0.0f,  0.0f, // 右下后 12
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   1.0f,  0.0f,  0.0f, // 右上后 13
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   1.0f,  0.0f,  0.0f, // 右上前 14
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,   1.0f,  0.0f,  0.0f, // 右下前 15

    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f,  1.0f,  0.0f, // 左上后 16
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f,  1.0f,  0.0f, // 右上后 17
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f,  1.0f,  0.0f, // 右上前 18
    -0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f,  1.0f,  0.0f, // 左上前 19

    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   0.0f, -1.0f,  0.0f, // 左下后 20
     0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   0.0f, -1.0f,  0.0f, // 右下后 21
     0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f,   0.0f, -1.0f,  0.0f, // 右下前 22
    -0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f,   0.0f, -1.0f,  0.0f  // 左下前 23
};

uint32_t indices[] = {
     0,  1,  2,  2,  3,  0, // 前
     4,  5,  6,  6,  7,  4, // 后
     8,  9, 10, 10, 11,  8, // 左
    12, 13, 14, 14, 15, 12, // 右
    16, 17, 18, 18, 19, 16, // 上
    20, 21, 22, 22, 23, 20  // 下
};

void Draw3DGraphic(GLFWwindow* window) {
    if (!shader) {
        shader = std::make_unique<Shader>("shader.vs", "shader.fs");
    }
    if (shader->objmap.find("rectangle") == shader->objmap.end()) {
        shader->newObject("rectangle");
    }
    Shader::ObjectInfo* obj = shader->getObject("rectangle");
    if (!obj->init) {
        glGenVertexArrays(1, &obj->VAO);
        glBindVertexArray(obj->VAO);
        glGenBuffers(1, &obj->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glGenBuffers(1, &obj->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
        glEnableVertexAttribArray(3);

        obj->newTexture("container.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, GL_RGB, false);
        obj->newTexture("awesomeface.png", GL_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, GL_RGBA, true);
        shader->use();
        shader->set1i("texture0", 0);
        shader->set1i("texture1", 1);

        obj->init = true;
    }
    shader->use();

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
    shader->set3fv("lightPos", 1, glm::value_ptr(lightPos));
    shader->set3fv("lightColor", 1, glm::value_ptr(lightColor));
    shader->set3fv("objectColor", 1, glm::value_ptr(objectColor));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->getTexture("container.jpg")->texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, obj->getTexture("awesomeface.png")->texture);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        shader->visibility = (shader->visibility + 0.01f > 1.0f ? 1.0f : shader->visibility + 0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        shader->visibility = (shader->visibility - 0.01f < 0.0f ? 0.0f : shader->visibility - 0.01f);
    }
    shader->set1f("visibility", shader->visibility);

    glBindVertexArray(obj->VAO);

    // for (int i = 0; i < cubPos.size(); i++) {
    //     cubPos[i].y = 0.0f;
    // }

    camera.ProcessKeyboard(window, shader->GetDeltaTime());

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
        // 法向量矩阵
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
        shader->setMatrix3fv("normalMatrix", 1, GL_FALSE, glm::value_ptr(normalMatrix));

        shader->setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        ImVec2 screenPos = WorldToScreen(cubPos[i], glm::mat4(1.0f), view, projection);
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(screenWidth / 2.0f, 0), screenPos, IM_COL32(255, 255, 0, 255), 1.0f);
        // ImGui::GetBackgroundDrawList()->AddCircle(screenPos, 10.0f, IM_COL32(255, 255, 0, 255), 0, 1.0f);
        char cubText[16];
        snprintf(cubText, sizeof(cubText), "Cub%d", i);
        ImVec2 textSize = ImGui::CalcTextSize(cubText);
        ImGui::GetBackgroundDrawList()->AddText(ImVec2(screenPos.x - textSize.x / 2, screenPos.y), IM_COL32(22, 255, 235, 255), cubText);
    }
}

#endif