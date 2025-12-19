#pragma once

#include "src/Resources/Shader.h"
// #include "src/Shader_old.h"

#ifdef _WIN32
    #define u_char unsigned char
#endif

void DrawTriangle();
void DrawRectangle();
void test1_DrawTwoTriangles();
void test2_DrawTwoTrianglesDiffData();
void test3_DrawTwoTrianglesDiffShader();

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec4 aColor;
    out vec4 vertexColor;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
        vertexColor = aColor;

        // vertexColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
)";
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    in vec4 vertexColor;
    // uniform vec4 ourColor;
    void main() {
        FragColor = vertexColor;

        // FragColor = ourColor;
        // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

struct ShaderInfo {
    GLuint shaderProgram;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
    const char* vertexShaderSource;
    const char* fragmentShaderSource;
    bool init = false;
};

ShaderInfo Tri;
ShaderInfo Rect;
ShaderInfo TTri;
ShaderInfo TTri1;
ShaderInfo TTri2;

GLuint compileShaderSource(GLenum type, const char* shaderSource, GLsizei count) {
    GLuint glShader = glCreateShader(type); // 创建着色器对象
    glShaderSource(glShader, count, &shaderSource, NULL); // 创建着色器
    glCompileShader(glShader); // 编译
    // 异常处理
    GLint success;
    char logInfo[512];
    glGetShaderiv(glShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(glShader, sizeof(logInfo), NULL, logInfo);
        std::cerr << "ShaderSouce compile error: " << logInfo << std::endl;
    }
    return glShader;
}

void DrawTriangle(int i) {
    if (!shader) {
        shader = std::make_unique<Shader>("shader.vs", "shader.fs");
    }
    if (shader->objmap.find("triangle") == shader->objmap.end()) {
        shader->newObject("triangle");
    }
    Shader::ObjectInfo* obj = shader->getObject("triangle");
    if (!obj->init) {
        float vertices[] = {
             0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             0.0f, -0.6f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f
        };

        glGenVertexArrays(1, &obj->VAO);
        glBindVertexArray(obj->VAO);
        glGenBuffers(1, &obj->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        obj->init = true;
    }
    shader->use();
    shader->set1f("offset", 0.5f);
    glBindVertexArray(obj->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


void DrawRectangle(GLFWwindow* window) {
    if (!shader) {
        shader = std::make_unique<Shader>("shader.vs", "shader.fs");
    }
    if (shader->objmap.find("rectangle") == shader->objmap.end()) {
        shader->newObject("rectangle");
    }
    Shader::ObjectInfo* obj = shader->getObject("rectangle");
    if (!obj->init) {
        float vertices[] = {
            //      坐标                 颜色                  纹理坐标
             0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,   1.0f,  1.0f, // 右上
             0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,   1.0f,  0.0f, // 右下
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,   0.0f,  0.0f, // 左下
            -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,   0.0f,  1.0f  // 左上
        };

        // for (int i = 0; i < sizeof(vertices) / sizeof(float); i = i + 9) {
        //     // vertices[i + 7] *= 2.0f;
        //     // vertices[i + 8] *= 2.0f;
        //     vertices[i + 7] /= 4.0f;
        //     vertices[i + 8] /= 4.0f;
        // }

        uint32_t indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &obj->VAO);
        glBindVertexArray(obj->VAO);
        glGenBuffers(1, &obj->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glGenBuffers(1, &obj->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
        glEnableVertexAttribArray(2);

        obj->newTexture("../res/container.jpg", GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_NEAREST, GL_NEAREST, GL_RGB, false);
        obj->newTexture("../res/awesomeface.png", GL_MIRRORED_REPEAT, GL_REPEAT, GL_LINEAR, GL_LINEAR, GL_RGBA, true);
        shader->use();
        shader->set1i("texture0", 0);
        shader->set1i("texture1", 1);

        obj->init = true;
    }
    shader->use();
    shader->set1f("offset", 0.0f);

    glm::mat4 trans;
    trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
    trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
    // trans = glm::scale(trans, glm::vec3(sin(glfwGetTime()), sin(glfwGetTime()), sin(glfwGetTime())));
    shader->setMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obj->getTexture("../res/container.jpg")->texture);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        shader->visibility = (shader->visibility + 0.01f > 1.0f ? 1.0f : shader->visibility + 0.01f);
    } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        shader->visibility = (shader->visibility - 0.01f < 0.0f ? 0.0f : shader->visibility - 0.01f);
    }
    shader->set1f("visibility", shader->visibility);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, obj->getTexture("../res/awesomeface.png")->texture);

    glBindVertexArray(obj->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // 绘制第二个图形
    trans = glm::mat4(1.0f);
    trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
    double scale = sin(glfwGetTime());
    scale = (scale < 0 ? -scale : scale);
    trans = glm::scale(trans, glm::vec3(scale, scale, scale));
    shader->setMatrix4fv("transform", 1, GL_FALSE, glm::value_ptr(trans));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}







void DrawTriangle() {
    if (!Tri.init) {
        // 动态编译 顶点着色器 片段着色器
        GLuint vertexShader =  compileShaderSource(GL_VERTEX_SHADER, vertexShaderSource, 1);
        GLuint fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER, fragmentShaderSource, 1);

        // 将着色器对象链接到程序对象
        Tri.shaderProgram = glCreateProgram();
        glAttachShader(Tri.shaderProgram, vertexShader);
        glAttachShader(Tri.shaderProgram, fragmentShader);
        glLinkProgram(Tri.shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // 定义顶点数据
        float vertices[] = {
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             0.0f,  0.6f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f
        };

        // 生成并绑定 VAO VBO
        glGenVertexArrays(1, &Tri.VAO);
        glBindVertexArray(Tri.VAO);
        glGenBuffers(1, &Tri.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Tri.VBO);

        // 将顶点数据复制到缓冲内存
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // 解析顶点数据 (顶点位置值, 顶点包含多少个值, 顶点数据类型, 是否标准化, 步长, 起始位置)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); // 位置属性
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float))); // 颜色属性
        glEnableVertexAttribArray(1);

        // 清除绑定
        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);

        Tri.init = true;
    }
    glUseProgram(Tri.shaderProgram);
    // int vertexColorLocation = glGetUniformLocation(Tri.shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, sin(glfwGetTime()) / 2.0f + 0.5f, 0.0f, 0.0f, 1.0f);
    glBindVertexArray(Tri.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void DrawRectangle() {
    if (!Rect.init) {
        // 动态编译 顶点着色器
        GLuint vertexShader = compileShaderSource(GL_VERTEX_SHADER, vertexShaderSource, 1); // 创建着色器对象

        // 动态编译 片段着色器
        GLuint fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER, fragmentShaderSource, 1);

        // 将着色器对象链接到程序对象
        Rect.shaderProgram = glCreateProgram();
        glAttachShader(Rect.shaderProgram, vertexShader);
        glAttachShader(Rect.shaderProgram, fragmentShader);
        glLinkProgram(Rect.shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        float vertices[] = {
            -0.5f,  0.5f, 0.0f, // 左上
            -0.5f, -0.5f, 0.0f, // 左下
             0.5f,  0.5f, 0.0f, // 右上
             0.5f, -0.5f, 0.0f  // 右下
        };

        uint32_t indices[] = {
            0, 1, 3,
            0, 2, 3
        };

        // 生成和绑定VAO
        glGenVertexArrays(1, &Rect.VAO);
        glBindVertexArray(Rect.VAO);
        // 生成和绑定VBO
        glGenBuffers(1, &Rect.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, Rect.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 生成和绑定EBO
        glGenBuffers(1, &Rect.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Rect.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // 解析顶点数据
        glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        Rect.init = true;
    }
    glUseProgram(Rect.shaderProgram);
    glBindVertexArray(Rect.VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void test1_DrawTwoTriangles() {
    if (!TTri.init) {
        GLuint vertexShader = compileShaderSource(GL_VERTEX_SHADER, vertexShaderSource, 1);
        GLuint fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER, fragmentShaderSource, 1);

        TTri.shaderProgram = glCreateProgram();
        glAttachShader(TTri.shaderProgram, vertexShader);
        glAttachShader(TTri.shaderProgram, fragmentShader);
        glLinkProgram(TTri.shaderProgram);
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        float vertices[] = {
             0.0f,  0.0f, 0.0f, 
             0.5f,  0.5f, 0.0f, 
             0.5f, -0.5f, 0.0f, 
             0.0f,  0.0f, 0.0f, 
            -0.5f,  0.5f, 0.0f, 
            -0.5f, -0.5f, 0.0f
        };

        glGenBuffers(1, &TTri.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, TTri.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &TTri.VAO);
        glBindVertexArray(TTri.VAO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // 解除绑定
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        TTri.init = true;
    }
    glUseProgram(TTri.shaderProgram);
    glBindVertexArray(TTri.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void test2_DrawTwoTrianglesDiffData() {
    if (!(TTri1.init && TTri2.init)) {
        GLuint vertexShader = compileShaderSource(GL_VERTEX_SHADER, vertexShaderSource, 1);
        GLuint fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER, fragmentShaderSource, 1);

        TTri1.shaderProgram = glCreateProgram();
        TTri2.shaderProgram = TTri1.shaderProgram;
        glAttachShader(TTri1.shaderProgram, vertexShader);
        glAttachShader(TTri1.shaderProgram, fragmentShader);
        glLinkProgram(TTri1.shaderProgram);
        
        float vertices[] = {
             0.0f,  0.0f, 0.0f, 
             0.5f,  0.5f, 0.0f, 
             0.5f, -0.5f, 0.0f, 
             0.0f,  0.0f, 0.0f, 
            -0.5f,  0.5f, 0.0f, 
            -0.5f, -0.5f, 0.0f
        };

        glGenVertexArrays(1, &TTri1.VAO);
        glBindVertexArray(TTri1.VAO);

        glGenBuffers(1, &TTri1.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, TTri1.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        glGenVertexArrays(1, &TTri2.VAO);
        glBindVertexArray(TTri2.VAO);

        glGenBuffers(1, &TTri2.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, TTri2.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);


        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        TTri1.init = true;
        TTri2.init = true;
    }
    glUseProgram(TTri1.shaderProgram);
    glBindVertexArray(TTri1.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(TTri2.VAO);
    glDrawArrays(GL_TRIANGLES, 3, 3);
}

void test3_DrawTwoTrianglesDiffShader() {
    if (!(TTri1.init && TTri2.init)) {
        TTri2.fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;
            void main() {
                FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
            }
        )";
        GLuint vertexShader = compileShaderSource(GL_VERTEX_SHADER, vertexShaderSource, 1);
        GLuint fragmentShader1 = compileShaderSource(GL_FRAGMENT_SHADER, fragmentShaderSource, 1);
        GLuint fragmentShader2 = compileShaderSource(GL_FRAGMENT_SHADER, TTri2.fragmentShaderSource, 1);

        TTri1.shaderProgram = glCreateProgram();
        TTri2.shaderProgram = glCreateProgram();
        glAttachShader(TTri1.shaderProgram, vertexShader);
        glAttachShader(TTri1.shaderProgram, fragmentShader1);
        glLinkProgram(TTri1.shaderProgram);
        glAttachShader(TTri2.shaderProgram, vertexShader);
        glAttachShader(TTri2.shaderProgram, fragmentShader2);
        glLinkProgram(TTri2.shaderProgram);

        float vertices[] = {
             0.0f,  0.0f, 0.0f, 
             0.5f,  0.5f, 0.0f, 
             0.5f, -0.5f, 0.0f, 
             0.0f,  0.0f, 0.0f, 
            -0.5f,  0.5f, 0.0f, 
            -0.5f, -0.5f, 0.0f
        };

        glGenVertexArrays(1, &TTri1.VAO);
        glBindVertexArray(TTri1.VAO);
        glGenBuffers(1, &TTri1.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, TTri1.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glGenVertexArrays(1, &TTri2.VAO);
        glBindVertexArray(TTri2.VAO);
        glGenBuffers(1, &TTri2.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, TTri2.VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        TTri1.init = true;
        TTri2.init = true;
    }
    glUseProgram(TTri1.shaderProgram);
    glBindVertexArray(TTri1.VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(TTri2.shaderProgram);
    glBindVertexArray(TTri2.VAO);
    glDrawArrays(GL_TRIANGLES, 3, 3);
}
