#include "RTRRenderer.h"

RTRRenderer::RTRRenderer()
{
    m_PinballStaticShader = new RTRShader();
    m_PinballStaticShader->Load("Src/RTRShaderTextured.vert", "Src/RTRShaderTextured.frag", "Src/RTRShaderTextured.geom");

    m_PinballDynamicShader = new RTRShader();
    m_PinballDynamicShader->Load("Src/RTRShaderTextured.vert", "Src/RTRShaderTextured.frag", "Src/RTRShaderTextured.geom");

    m_DynamicObjectsShader = new RTRShader();
    m_DynamicObjectsShader->Load("Src/RTRShaderTextured.vert", "Src/RTRShaderTextured.frag", "Src/RTRShaderTextured.geom");

    m_SkyboxShader = new RTRShader();
    m_SkyboxShader->Load("Src/RTRSkyboxShader.vert", "Src/RTRSkyboxShader.frag");

    m_UniformGridShader = new RTRShader();
    m_UniformGridShader->Load("Src/RTRShaderNoTexture.vert", "Src/RTRShaderNoTexture.frag", "Src/RTRShaderNoTexture.geom");

    glUseProgram(m_SkyboxShader->GetId());
    m_SkyboxShader->SetInt("skybox", 0);

    ShaderVector.push_back(m_PinballStaticShader);
    ShaderVector.push_back(m_PinballDynamicShader);
    ShaderVector.push_back(m_DynamicObjectsShader);
    ShaderVector.push_back(m_SkyboxShader);
    ShaderVector.push_back(m_UniformGridShader);

    lastTime = 0.0f;
    timer = 0.0f;
}

void RTRRenderer::SetUp() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RTRRenderer::ObjectTransformation(RTRShader* shader, glm::mat4 modelMatrix, glm::vec3 translation, 
    glm::vec3 scale, glm::vec3 rotation) {

    modelMatrix = glm::rotate(modelMatrix, 6.5f, rotation);
    modelMatrix = glm::translate(modelMatrix, translation);
    modelMatrix = glm::scale(modelMatrix, scale);

    //float now = SDL_GetTicks();
    //float delta = now - lastTime;
    //lastTime = now;
    //timer += delta;

    //modelMatrix = glm::rotate(modelMatrix, timer / 1000.0f, rotation);

    shader->SetMat4("u_ModelMatrix", modelMatrix);
}

void RTRRenderer::RenderWithShaders(int shaderPos, glm::mat4 modelMatrix, glm::mat4 viewMatrix,
    glm::mat4 projectionMatrix, RTRObject* object, RTRCamera* camera, RTRLightingModel* lightingModel,
    int curTime, int timeDelta) {

    glUseProgram(ShaderVector.at(shaderPos)->GetId());
    ShaderVector.at(shaderPos)->SetFloat("u_CurTime", (float)curTime);
    ShaderVector.at(shaderPos)->SetFloat("u_TimeDelta", (float)timeDelta);
    ShaderVector.at(shaderPos)->SetMat4("u_ViewMatrix", viewMatrix);
    ShaderVector.at(shaderPos)->SetMat4("u_ProjectionMatrix", projectionMatrix);
    ShaderVector.at(shaderPos)->SetCamera("u_Camera", *camera);
    ShaderVector.at(shaderPos)->SetLightingModel(*lightingModel);

    /*ObjectTransformation(ShaderVector.at(shaderPos), modelMatrix, translation, scale, rotation);*/

    ShaderVector.at(shaderPos)->SetMat4("u_ModelMatrix", object->GetTransformMatrix());

    object->Render(ShaderVector.at(shaderPos));
}

void RTRRenderer::RenderSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
    glUseProgram(ShaderVector.at(3)->GetId());
    ShaderVector.at(3)->SetMat4("u_ViewMatrix", viewMatrix);
    ShaderVector.at(3)->SetMat4("u_ProjectionMatrix", projectionMatrix);
}

void RTRRenderer::DebugInfo(Console* console, int FPS, RTRCamera* camera) {
    // Print out all debug info
    console->Render("DEBUG", FPS,
        camera->m_Position.x, camera->m_Position.y, camera->m_Position.z,
        camera->m_Yaw, camera->m_Pitch);
}

void RTRRenderer::Done() {
    ShaderVector.clear();

    if (m_PinballStaticShader != nullptr) { delete m_PinballStaticShader; m_PinballStaticShader = nullptr; }
    if (m_PinballDynamicShader != nullptr) { delete m_PinballDynamicShader; m_PinballDynamicShader = nullptr; }
    if (m_DynamicObjectsShader != nullptr) { delete m_DynamicObjectsShader; m_DynamicObjectsShader = nullptr; }
    if (m_SkyboxShader != nullptr) { delete m_SkyboxShader; m_SkyboxShader = nullptr; }
    if (m_UniformGridShader != nullptr) { delete m_UniformGridShader; m_UniformGridShader = nullptr; }

    lastTime = 0;
    timer = 0;
}