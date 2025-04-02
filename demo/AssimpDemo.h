#pragma once

#include "utils/Shader.h"
// #include "utils/Shader_old.h"
#include "utils/Camera.h"
#include "utils/Model.h"

extern int screenWidth;
extern int screenHeight;

void RenderModel(GLFWwindow* window, Shader& shader, Model& _model) {
    camera.ProcessKeyboard(window, shader.GetDeltaTime());
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
              model = glm::translate(model, glm::vec3(0.0f, -3.0f, -3.0f));
              model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader.setMatrix4fv("model", model);
    shader.setMatrix4fv("view", view);
    shader.setMatrix4fv("projection", projection);
    _model.Draw(shader);
}
