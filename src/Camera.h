#pragma once

#include "MMath.h"

float PITCH = 0.0f;
float YAW = -90.0f;
float ZOOM = 45.0f;
float MOVESPEED = 4.5f;
float MOUSESENSITIVE = 0.08f;

class Camera {
public:
    bool UpdateMouseMv = false;
    bool UpdateKeyboard = true;

    float Pitch;
    float Yaw;
    float Zoom;
    float MoveSpeed;
    float MouseSensitive;
    
    glm::vec3 Pos;
    glm::vec3 Front;
    glm::vec3 Right;
    glm::vec3 Up;

    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 3.0f), float pitch = PITCH, float yaw = YAW) : Zoom(ZOOM), MoveSpeed(MOVESPEED), MouseSensitive(MOUSESENSITIVE) {
            Pos = pos;
            Pitch = pitch;
            Yaw = yaw;

            UpdataCameraVectors();
    }
    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Pos, Pos + Front, Up);
    }
    void ProcessKeyboard(GLFWwindow* window, float deltaTime) {
        if (UpdateKeyboard) {
            float velocity = MoveSpeed * deltaTime;
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                Pos += Front * velocity;
            }
            if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                Pos -= Front * velocity;
            }
            if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                Pos += glm::normalize(glm::cross(Front, Up)) * velocity;
            }
            if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                Pos -= glm::normalize(glm::cross(Front, Up)) * velocity;
            }
        }
        // Pos.y = 0.0f;
    }
    void ProcessMouseMovement(float xOffset, float yOffset) {
        xOffset *= MouseSensitive;
        yOffset *= MouseSensitive;

        if (UpdateMouseMv) {
            Pitch += yOffset;
            Yaw   += xOffset;
        }

        if (Pitch > 89.0f) Pitch = 89.0f;
        else if (Pitch < -89.0f) Pitch = -89.0f;

        UpdataCameraVectors();
    }
    void ProcessMouseScrool(float yOffset) {
        Zoom -= (yOffset / 3.0f);
        if (Zoom < 1.0f) Zoom = 1.0f;
        else if (Zoom > 45.0f) Zoom = 45.0f;
    }

private:
    void UpdataCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
        front.y = sin(glm::radians(Pitch));
        front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, glm::vec3(0.0f, 1.0f, 0.0f)));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

Camera camera(glm::vec3(0.0f, 1.0f, 5.0f));
