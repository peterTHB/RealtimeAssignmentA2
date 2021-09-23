#include "RTRRenderer.h"

RTRRenderer::RTRRenderer()
{
}

void RTRRenderer::SetUp() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RTRRenderer::RenderWithShaders(RTRShader* shader, glm::mat4 modelMatrix, glm::mat4 viewMatrix,
    glm::mat4 projectionMatrix, RTRObject* object, RTRCamera* camera, RTRLightingModel* lightingModel,
    int curTime, int timeDelta, bool change) {

    glUseProgram(shader->GetId());
    shader->SetFloat("u_CurTime", (float)curTime);
    shader->SetFloat("u_TimeDelta", (float)timeDelta);
    shader->SetMat4("u_ViewMatrix", viewMatrix);
    shader->SetMat4("u_ProjectionMatrix", projectionMatrix);
    shader->SetCamera("u_Camera", *camera);
    shader->SetLightingModel(*lightingModel);

    if (change) {
        modelMatrix = glm::translate(modelMatrix, glm::vec3(3.0, 0.0, 0.0));
    }
    shader->SetMat4("u_ModelMatrix", modelMatrix);

    object->Render(shader);
}

void RTRRenderer::DebugInfo(Console* console, int FPS, RTRCamera* camera) {
    // Print out all debug info
    console->Render("DEBUG", FPS,
        camera->m_Position.x, camera->m_Position.y, camera->m_Position.z,
        camera->m_Yaw, camera->m_Pitch);
}
