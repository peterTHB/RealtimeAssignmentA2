#include "RTRCamera.h"



void RTRCamera::Mouse_Callback(float xRel, float yRel) {

    float sensitivity = 0.05f;

    m_Yaw += (xRel * sensitivity);
    m_Pitch += (-yRel * sensitivity);

    LockCamera();
    UpdateVectors();
}

void RTRCamera::LockCamera()
{
    if (m_Pitch > 89.9f) {
        m_Pitch = 89.9f;
    }
    if (m_Pitch < -89.9f) {
        m_Pitch = -89.9f;
    }

    if (m_Yaw < 0.0) {
        m_Yaw += 360.0f;
    }

    if (m_Yaw > 360.0f) {
        m_Yaw -= 360.0f;
    }
}

void RTRCamera::ResetPosition() {
    m_Position = glm::vec3(0.0, 11.0, 13.0);
    m_Pitch = DEFAULT_PITCH;
    m_Yaw = DEFAULT_YAW;

    LockCamera();
    UpdateVectors();
}

void RTRCamera::SetCameraPos(glm::vec3 cameraPos)
{
    this->m_Position = cameraPos;
}

glm::vec3* RTRCamera::GetCameraPos()
{
    glm::vec3* cameraNum = &m_Position;
    return cameraNum;
}

glm::vec3* RTRCamera::GetCameraFront()
{
    glm::vec3* cameraNum = &m_Front;
    return cameraNum;
}

glm::vec3* RTRCamera::GetCameraUp()
{
    glm::vec3* cameraNum = &m_Up;
    return cameraNum;
}

const float* RTRCamera::GetCameraSpeed() {
    const float* speed = &cameraSpeed;
    return speed;
}

float* RTRCamera::GetDeltaTime()
{
    float currentFrame = SDL_GetTicks();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    deltaTime /= 1000;

    float* deltaPtr = &deltaTime;
    return deltaPtr;
}