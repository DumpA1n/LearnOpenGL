#pragma once

#include "src/Shader.h"
#include "src/Camera.h"
#include "src/Model.h"

void RenderModel(GLFWwindow* window, Shader& shader, Model& _model) {
    camera.ProcessKeyboard(window, shader.getDeltaTime());
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
              model = glm::translate(model, glm::vec3(0.0f, -3.0f, -3.0f));
              model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    shader.setMatrix4fv("model", glm::value_ptr(model));
    shader.setMatrix4fv("view", glm::value_ptr(view));
    shader.setMatrix4fv("projection", glm::value_ptr(projection));
    _model.Draw(shader);
}
