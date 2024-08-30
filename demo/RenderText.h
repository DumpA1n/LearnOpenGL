#pragma once
#ifndef RENDERTEXT_H
#define RENDERTEXT_H

#include <algorithm>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

#include "Shader/Shader.h"
#include "Camera/Camera.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "stb/stb_truetype.h"

struct Font {

    struct Character {
        GLuint     TexID;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        GLuint     Advance;
    };
    std::map<uint32_t, Character*> chmap;

    void AddFontFromFileTTF(const char* filename, FT_UInt pixel_width, FT_UInt pixel_height, bool use_sdf) {
        FT_Library ft;
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "[!] Failed to init FreeType" << std::endl;
        }
        FT_Face face;
        if (FT_New_Face(ft, filename, 0, &face)) {
            std::cerr << "[!] Failed to load font" << std::endl;
        }

        FT_Set_Pixel_Sizes(face, pixel_width, pixel_height);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制

        int advancex = INT_MIN; // 文本宽度对齐

        FT_GlyphSlot slot = face->glyph;
        for (GLuint ch = 0; ch < 128; ch++) {
            if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
                std::cerr << "[!] Failed to load char" << std::endl;
            }
            if (use_sdf)
                FT_Render_Glyph(slot, FT_RENDER_MODE_SDF);

            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, slot->bitmap.width, slot->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, slot->bitmap.buffer);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            auto character = new Character();
            character->TexID = texture;
            character->Size = glm::ivec2(slot->bitmap.width, slot->bitmap.rows);
            character->Bearing = glm::ivec2(slot->bitmap_left, slot->bitmap_top);
            character->Advance = std::max(advancex, (int)slot->advance.x);
            chmap.insert({ch, character});
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }
};

Font font;
Font font_sdf;









glm::vec3 text_pos(0.0f, 0.0f, 0.0f);
glm::vec3 text_rotate(0.0f, 0.0f, 0.0f);
glm::vec3 text_scale(1.0f, 1.0f, 1.0f);


void RenderText(std::string text, glm::vec2 pos, glm::vec4 color, GLfloat scale, bool use_sdf) {
    glDepthMask(GL_FALSE);
    Shader *shader = (use_sdf ? cshader_sdf.get() : cshader_smooth.get());
    if (!shader) {
        if (use_sdf) {
            cshader_sdf = std::make_unique<Shader>("../res/CharacterShader.vs", "../res/CharacterShaderSDF.fs");
        } else {
            cshader_smooth = std::make_unique<Shader>("../res/CharacterShader.vs", "../res/CharacterShaderSmooth.fs");
        }
    }
    shader = (use_sdf ? cshader_sdf.get() : cshader_smooth.get());
    if (shader->getObject("character") == nullptr) {
        shader->newObject("character");
    }
    Shader::ObjectInfo *obj = shader->getObject("character");
    if (!obj->init) {
        GLuint indices[2][3] = {
            0, 1, 3,
            1, 2, 3
        };
        glGenVertexArrays(1, &obj->VAO);
        glBindVertexArray(obj->VAO);
        glGenBuffers(1, &obj->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        glGenBuffers(1, &obj->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        obj->init = true;
    }

    // glm::mat4 model = glm::mat4(1.0f);
    //           model = glm::scale(model, text_scale);
    //           model = glm::rotate(model, (float)glfwGetTime(), text_rotate);
    //           model = glm::translate(model, text_pos);
    // glm::mat4 view = camera.GetViewMatrix();
    // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(screenWidth), 0.0f, static_cast<float>(screenHeight));

    shader->use();
    shader->set1f("time", (float)glfwGetTime());
    shader->set4fv("textColor", 1, glm::value_ptr(color));
    // shader->setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
    // shader->setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
    shader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(obj->VAO);

    // pos.y = screenHeight - pos.y;

    for (const auto& c : text) {
        auto ch = font.chmap[c];
        if (use_sdf) ch = font_sdf.chmap[c];

        GLfloat x = pos.x + ch->Bearing.x * scale;
        GLfloat y = pos.y - (ch->Size.y - ch->Bearing.y) * scale;
        GLfloat w = ch->Size.x * scale;
        GLfloat h = ch->Size.y * scale;

        printf("xywh(%.2f, %.2f, %.2f, %.2f)\n", x, y, pos.x, pos.y);

        // x /= screenWidth;
        // y /= screenHeight;
        // w /= screenWidth;
        // h /= screenHeight;

        GLfloat vertices[] = {
            x,     y + h, 0.0f,  0.0f, 0.0f , // 左下
            x,     y,     0.0f,  0.0f, 1.0f , // 左上
            x + w, y,     0.0f,  1.0f, 1.0f , // 右上
            x + w, y + h, 0.0f,  1.0f, 0.0f   // 右下
        };

        glBindTexture(GL_TEXTURE_2D, ch->TexID);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        pos.x += (ch->Advance >> 6) * scale;
    }
    glDepthMask(GL_TRUE);
}








void RenderText(std::string text, glm::vec2 pos, glm::vec4 color, GLfloat scale) {
    glDepthMask(GL_FALSE);
    Shader *shader = cshader.get();
    if (!shader) {
        cshader = std::make_unique<Shader>("../res/CharacterShader.vs", "../res/CharacterShader.fs");
    }
    shader = cshader.get();
    if (shader->getObject("character") == nullptr) {
        shader->newObject("character");
    }
    Shader::ObjectInfo *obj = shader->getObject("character");
    if (!obj->init) {
        GLuint indices[2][3] = {
            0, 1, 3,
            1, 2, 3
        };
        glGenVertexArrays(1, &obj->VAO);
        glBindVertexArray(obj->VAO);
        glGenBuffers(1, &obj->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        glGenBuffers(1, &obj->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        obj->init = true;
    }

    glm::mat4 model = glm::mat4(1.0f);
              model = glm::translate(model, lightPos);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

    shader->use();
    shader->set1f("time", (float)glfwGetTime());
    shader->set4fv("textColor", 1, glm::value_ptr(color));
    shader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));
    shader->setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
    shader->setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(obj->VAO);

    // pos.y = screenHeight - pos.y;

    for (const auto& c : text) {
        auto ch = font.chmap[c];
        ch = font.chmap[c];

        GLfloat x = pos.x + ch->Bearing.x * scale;
        GLfloat y = pos.y - (ch->Size.y - ch->Bearing.y) * scale;
        GLfloat w = ch->Size.x * scale;
        GLfloat h = ch->Size.y * scale;

        x /= screenWidth;
        y /= screenHeight;
        w /= screenWidth;
        h /= screenHeight;
        
        GLfloat vertices[] = {
            x,     y + h, 0.0f,  0.0f, 0.0f , // 左下
            x,     y,     0.0f,  0.0f, 1.0f , // 左上
            x + w, y,     0.0f,  1.0f, 1.0f , // 右上
            x + w, y + h, 0.0f,  1.0f, 0.0f   // 右下
        };

        glBindTexture(GL_TEXTURE_2D, ch->TexID);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        pos.x += (ch->Advance >> 6) * scale;
    }
    glDepthMask(GL_TRUE);
}








void RenderText(std::string text, glm::vec3 pos, glm::vec4 color, GLfloat scale, bool use_sdf) {
    glDepthMask(GL_FALSE);
    Shader *shader = (use_sdf ? cshader_sdf.get() : cshader_smooth.get());
    if (!shader) {
        if (use_sdf) {
            cshader_sdf = std::make_unique<Shader>("../res/CharacterShader.vs", "../res/CharacterShaderSDF.fs");
        } else {
            cshader_smooth = std::make_unique<Shader>("../res/CharacterShader.vs", "../res/CharacterShaderSmooth.fs");
        }
    }
    shader = (use_sdf ? cshader_sdf.get() : cshader_smooth.get());
    if (shader->getObject("character") == nullptr) {
        shader->newObject("character");
    }
    Shader::ObjectInfo *obj = shader->getObject("character");
    if (!obj->init) {
        GLuint indices[2][3] = {
            0, 1, 3,
            1, 2, 3
        };
        glGenVertexArrays(1, &obj->VAO);
        glBindVertexArray(obj->VAO);
        glGenBuffers(1, &obj->VBO);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
        glGenBuffers(1, &obj->EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        obj->init = true;
    }

    glm::mat4 model = glm::mat4(1.0f);
              model = glm::translate(model, pos);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

    pos = projection * view * model * glm::vec4(pos, 1.0f);
    printf("pos(%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);

    shader->use();
    shader->set1f("time", (float)glfwGetTime());
    shader->set4fv("textColor", 1, glm::value_ptr(color));
    shader->setMatrix4fv("model", 1, GL_FALSE, glm::value_ptr(model));
    shader->setMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(view));
    shader->setMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(projection));

    // pos.y = screenHeight - pos.y;

    for (const auto& c : text) {
        auto ch = font.chmap[c];
        if (use_sdf) ch = font_sdf.chmap[c];

        GLfloat x = pos.x + ch->Bearing.x * scale;
        GLfloat y = pos.y - (ch->Size.y - ch->Bearing.y) * scale;
        GLfloat w = ch->Size.x * scale;
        GLfloat h = ch->Size.y * scale;

        x /= screenWidth;
        y /= screenHeight;
        w /= screenWidth;
        h /= screenHeight;

        // printf("xywh(%.2f, %.2f, %.2f, %.2f)\n", x, y, pos.x, pos.y);

        GLfloat vertices[] = {
            x,     y + h, 0.0f,  0.0f, 0.0f , // 左下
            x,     y,     0.0f,  0.0f, 1.0f , // 左上
            x + w, y,     0.0f,  1.0f, 1.0f , // 右上
            x + w, y + h, 0.0f,  1.0f, 0.0f   // 右下
        };

        glBindTexture(GL_TEXTURE_2D, ch->TexID);
        glBindBuffer(GL_ARRAY_BUFFER, obj->VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        pos.x += (ch->Advance >> 6) * scale;
    }
    glDepthMask(GL_TRUE);
}

#endif