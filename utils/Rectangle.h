#pragma once

#include "Object.h"

struct Rectangle : Object {
public:
    Rectangle() {
        static float vertices[] = {
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
        
        static uint32_t indices[] = {
             0,  1,  2,  2,  3,  0, // 前
             4,  5,  6,  6,  7,  4, // 后
             8,  9, 10, 10, 11,  8, // 左
            12, 13, 14, 14, 15, 12, // 右
            16, 17, 18, 18, 19, 16, // 上
            20, 21, 22, 22, 23, 20  // 下
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(9 * sizeof(float)));
        glEnableVertexAttribArray(3);
    }

    void gen() { }
};