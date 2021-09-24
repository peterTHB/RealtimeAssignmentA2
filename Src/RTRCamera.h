//-----------------------------------------------------------------------------
// RTRCamera.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0f, 0.0f, 0.0f);
static const glm::vec3 DEFAULT_FRONT    = glm::vec3(0.0f, 0.0f, -1.0f);
static const glm::vec3 DEFAULT_UP       = glm::vec3(0.0f, 1.0f, 0.0f);
static const glm::vec3 DEFAULT_RIGHT    = glm::vec3(1.0f, 0.0f, 0.0f);
static const float DEFAULT_YAW          = 270.0f;        // degrees -90.0f
static const float DEFAULT_PITCH        = -50.0f;        // degrees 0.0f
static const float DEFAULT_MOVE_SPEED   = 10.0f;          // units per second
static const float DEFAULT_STRAFE_SPEED = 4.0f;          // units per second
static const float DEFAULT_TURN_SPEED   = 60.0f;         // degrees per second
static const float DEFAULT_TILT_SPEED   = 60.0f;         // degrees per second

class RTRCamera
{
public:
    RTRCamera(glm::vec3 position = DEFAULT_POSITION, glm::vec3 up = DEFAULT_UP, float yaw = DEFAULT_YAW, float pitch = DEFAULT_PITCH) 
        : m_Position{ position }
        , m_WorldUp{ up }
        , m_Yaw{ yaw }
        , m_Pitch{ pitch }
    { UpdateVectors(); }

    // TODO: Add code to stop camera from flipping
    // TODO: move time multiplication out of this class?? Keep camera simple, and physics separate...
    glm::mat4 GetViewMatrix() { return glm::lookAt(m_Position, m_Position + m_Front, m_Up); }

    // TODO: make private; add public getters and setters for all of these
    glm::vec3 m_Position;
    glm::vec3 m_WorldUp;
    glm::vec3 m_Front{ DEFAULT_FRONT };
    glm::vec3 m_Up{ DEFAULT_UP };
    glm::vec3 m_Right{ DEFAULT_RIGHT };
    float m_Yaw;
    float m_Pitch;
    float m_MoveSpeed{ DEFAULT_MOVE_SPEED };
    float m_StrafeSpeed{ DEFAULT_STRAFE_SPEED };
    float m_TurnSpeed{ DEFAULT_TURN_SPEED };
    float m_TiltSpeed{ DEFAULT_TILT_SPEED };

    // Personal Implementation
    virtual void Mouse_Callback(float xRel, float yRel);
    virtual void LockCamera();
    virtual void ResetPosition();
    virtual void SetCameraPos(glm::vec3 cameraPos);
    virtual glm::vec3* GetCameraPos();
    virtual glm::vec3* GetCameraFront();
    virtual glm::vec3* GetCameraUp();
    virtual const float* GetCameraSpeed();
    virtual float* GetDeltaTime();
    float cameraSpeed{ DEFAULT_MOVE_SPEED };
    float deltaTime{ 0.0f };
    float lastFrame{ 0.0f };

private:
    void UpdateVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);

        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }
};
