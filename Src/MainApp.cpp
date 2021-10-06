//-----------------------------------------------------------------------------
// MainApp.cpp
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#include "MainApp.h"
#include "RTRShader.h"

int MainApp::Init()
{
    if (int err = RTRApp::Init() != 0) {
        return err;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_GetWindowSize(m_SDLWindow, &m_WindowWidth, &m_WindowHeight);
    SDL_WarpMouseInWindow(m_SDLWindow, m_WindowWidth, m_WindowHeight);

    glEnable(GL_DEPTH_TEST);

    // Setup projection matrix and viewport transform. 
    // These won't need to change as we're not worrying about screen size changes for this assignment
    m_ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);

    m_ModelMatrix = glm::mat4(1.0f);

    // Create and initialise camera
    m_Camera = new RTRCamera(glm::vec3(0.0, 11.0, 13.0), glm::vec3(0.0, 1.0, 0.0));

    m_RTRWorld = new RTRWorld(m_ModelMatrix);
    m_RTRPhysicsEngine = new RTRPhysicsEngine(m_RTRWorld);
    m_RTRRenderer = new RTRRenderer();

    // Create and initialise the debug console/overlay
    m_Console = new Console();
    m_Console->Init();

    return 0;
}

void MainApp::Done()
{
    m_RTRWorld->Done();
    delete m_RTRWorld;

    m_RTRRenderer->Done();
    delete m_RTRRenderer;
    
    m_RTRPhysicsEngine->Done();
    delete m_RTRPhysicsEngine;

    m_Console->End(); delete m_Console;

    RTRApp::Done();
}

bool MainApp::Tick()
{
    m_QuitApp = false;
    CheckInput();
    UpdateState(m_TimeDelta);
    RenderFrame();
    return m_QuitApp;
}

// TODO: Add MOUSE support for looking around.
// TODO: Might move all this as "core" functionality in RTRApp class.
void MainApp::CheckInput()
{
    SDL_Event e;

    glm::vec3 cameraPos = *m_Camera->GetCameraPos();
    glm::vec3 cameraFront = *m_Camera->GetCameraFront();
    glm::vec3 cameraUp = *m_Camera->GetCameraUp();
    float cameraDeltaTime = *m_Camera->GetDeltaTime();
    const float cameraSpeed = *m_Camera->GetCameraSpeed() * cameraDeltaTime;
    float xRel = 0;
    float yRel = 0;

    while (SDL_PollEvent(&e) != 0) {
        switch (e.type) {
            case SDL_QUIT: m_QuitApp = true; break;

            case SDL_MOUSEMOTION:
                xRel = e.motion.xrel;
                yRel = e.motion.yrel;
                m_Camera->Mouse_Callback(xRel, yRel);
                break;

            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: m_QuitApp = true; break;

                    // Camera Controls
                    case SDLK_w:
                        cameraPos += cameraSpeed * cameraFront;
                        m_Camera->SetCameraPos(cameraPos);
                        break;

                    case SDLK_s:
                        cameraPos -= cameraSpeed * cameraFront;
                        m_Camera->SetCameraPos(cameraPos);
                        break;

                    case SDLK_a:
                        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                        m_Camera->SetCameraPos(cameraPos);
                        break;

                    case SDLK_d:
                        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
                        m_Camera->SetCameraPos(cameraPos);
                        break;

                    case SDLK_r:
                        m_Camera->ResetPosition();
                        break;

                    // Plunger increase tension
                    case SDLK_UP:
                        m_UsePlunger = true;
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    // Plunger release
                    case SDLK_UP:
                        m_UsePlunger = false;
                        /*m_ShootBall = true;*/
                        break;

                }
                break;
        }
    }
}

void MainApp::UpdateState(unsigned int td_milli)
{
    // Update directional camera to align with camera forward direction
    m_RTRWorld->GetLightingModel()->GetLight(0)->Direction = m_Camera->m_Front;

    // Setup Model and View matrices
    m_ModelMatrix = glm::mat4(1.0f);
    m_ViewMatrix = m_Camera->GetViewMatrix();
    //m_ViewMatrix = glm::mat4(glm::mat3(m_Camera->GetViewMatrix()));

    // Shoot currentball in dynamic objects vector
    //if (m_ShootBall) {
    //    RTRObject* currBall = m_RTRWorld->GetDynamicObjects().at(m_NumBallsShot);
    //    // Apply velocity here
    //    currBall->SetVelocity(m_RTRPhysicsEngine->GetPower());
    //    m_ShootBall = false;
    //}
}

void MainApp::RenderFrame()
{
    m_RTRRenderer->SetUp();

    // For all static pinball objects
    for (RTRObject* staticPBObject : m_RTRWorld->GetStaticPinballObjects()) {
        //glm::vec3 transform = staticPBObject->GetTransform();
        //glm::vec3 scale = staticPBObject->GetScale();
        //glm::vec3 rotation = staticPBObject->GetRotation();

        m_RTRRenderer->RenderWithShaders(0, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
            staticPBObject, m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
    }

    // For all other dynamic objects, i.e: pinball ball
    // Have method to make new sphere
    for (RTRObject* dynaObject : m_RTRWorld->GetDynamicObjects()) {
        //glm::vec3 transform = dynaObject->GetTransform();
        //glm::vec3 scale = dynaObject->GetScale();
        //glm::vec3 rotation = dynaObject->GetRotation();

        m_RTRRenderer->RenderWithShaders(2, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
            dynaObject, m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
    }

    // For all dynamic pinball objects
    //glm::vec3 transform = m_RTRWorld->GetDynamicPinballObjects().at(0)->GetTransform();
    //glm::vec3 scale = m_RTRWorld->GetDynamicPinballObjects().at(0)->GetScale();
    //glm::vec3 rotation = m_RTRWorld->GetDynamicPinballObjects().at(0)->GetRotation();
    glm::mat4 position = m_RTRWorld->GetDynamicPinballObjects().at(0)->GetPosition();

    m_RTRWorld->GetDynamicPinballObjects().at(0)->SetPosition(m_RTRPhysicsEngine->UsePlunger(m_UsePlunger, m_TimeDelta, position));

    m_RTRRenderer->RenderWithShaders(1, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_RTRWorld->GetDynamicPinballObjects().at(0), m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);

    if (!m_UsePlunger && !m_ShootBall) m_RTRPhysicsEngine->ResetPower();

    m_RTRRenderer->DebugInfo(m_Console, m_FPS, m_Camera);

    // Drawing skybox
    glm::mat4 skyboxView = glm::mat4(glm::mat3(m_Camera->GetViewMatrix()));
    m_RTRRenderer->RenderSkybox(skyboxView, m_ProjectionMatrix);
    m_RTRWorld->DrawSkybox();

    // Swap buffers
    SDL_GL_SwapWindow(m_SDLWindow);
}

//-----------------------------------------------------------------------------
static const char* MAIN_WINDOW_TITLE = "RMIT COSC1226: Real-Time Rendering";
int main(int argc, char** argv)
{
    MainApp* app = new MainApp(MAIN_WINDOW_TITLE, false, 1200, 800);
    if (int err = app->Init() != 0) {
        std::cout << "RTR:ERROR: Failed to initialise application. Please see error messages for details..." << std::endl;
        return -1;
    };
    app->Run();
    app->Done();
    return 0;
}
