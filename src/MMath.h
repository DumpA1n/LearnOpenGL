#pragma once

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

extern int screenWidth, screenHeight;

inline ImVec2 WorldToScreen(const glm::vec3& pos, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
    glm::vec4 clipSpacePos = projection * view * model * glm::vec4(pos, 1.0);
    if (clipSpacePos.w > 0) {
        clipSpacePos /= clipSpacePos.w;
    }
    ImVec2 screenPos;
    screenPos.x = (clipSpacePos.x * 0.5f + 0.5f) * screenWidth;
    screenPos.y = (1.0f - clipSpacePos.y * 0.5f - 0.5f) * screenHeight;
    return screenPos;
}
