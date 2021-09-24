#include "RTRRenderer.h"

RTRRenderer::RTRRenderer()
{
    lastTime = 0.0f;
    timer = 0.0f;
}

void RTRRenderer::SetUp() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void RTRRenderer::ObjectTransformation(RTRShader* shader, glm::mat4 modelMatrix, glm::vec3 translation, 
    glm::vec3 scale, glm::vec3 rotation) {
    modelMatrix = glm::translate(modelMatrix, translation);
    modelMatrix = glm::scale(modelMatrix, scale);

    //float now = SDL_GetTicks();
    //float delta = now - lastTime;
    //lastTime = now;
    //timer += delta;

    //modelMatrix = glm::rotate(modelMatrix, timer / 1000.0f, rotation);

    shader->SetMat4("u_ModelMatrix", modelMatrix);
}

void RTRRenderer::RenderWithShaders(RTRShader* shader, glm::mat4 modelMatrix, glm::mat4 viewMatrix,
    glm::mat4 projectionMatrix, RTRObject* object, RTRCamera* camera, RTRLightingModel* lightingModel,
    int curTime, int timeDelta, glm::vec3 translation, glm::vec3 scale, glm::vec3 rotation) {

    glUseProgram(shader->GetId());
    shader->SetFloat("u_CurTime", (float)curTime);
    shader->SetFloat("u_TimeDelta", (float)timeDelta);
    shader->SetMat4("u_ViewMatrix", viewMatrix);
    shader->SetMat4("u_ProjectionMatrix", projectionMatrix);
    shader->SetCamera("u_Camera", *camera);
    shader->SetLightingModel(*lightingModel);

    ObjectTransformation(shader, modelMatrix, translation, scale, rotation);

    object->Render(shader);
}

void RTRRenderer::DebugInfo(Console* console, int FPS, RTRCamera* camera) {
    // Print out all debug info
    console->Render("DEBUG", FPS,
        camera->m_Position.x, camera->m_Position.y, camera->m_Position.z,
        camera->m_Yaw, camera->m_Pitch);
}
