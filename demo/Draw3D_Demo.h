#pragma once

#include <memory> // unique_ptr
#include <thread>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "src/Shader.h"

void DrawCube() {
    static Shader* shader = new Shader("../res/shader/cube/shader.vs", "../res/shader/cube/shader.fs");

    float vertices[] = {
         0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
         0.0f, -0.6f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &shader->VAO);
    glBindVertexArray(shader->VAO);
    glGenBuffers(1, &shader->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, shader->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);

    glUseProgram(shader->program);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}