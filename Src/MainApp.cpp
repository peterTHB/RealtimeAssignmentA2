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
                    case SDLK_SPACE:
                        m_UsePlunger = true;
                        break;

                    // Flippers
                    case SDLK_LEFT:
                        m_UseLeftFlipper = true;
                        break;

                    case SDLK_RIGHT:
                        m_UseRightFlipper = true;
                        break;

                    // Increase/Decrease table angle
                    //case SDLK_UP:
                    //    m_TableAngleUp = true;
                    //    break;

                    //case SDLK_DOWN:
                    //    m_TableAngleDown = true;
                    //    break;

                    // Debug mode toggle
                    case SDLK_p:
                        m_DebugModeOn = !m_DebugModeOn;
                        break;
                }
                break;

            case SDL_KEYUP:
                switch (e.key.keysym.sym) {
                    // Plunger release
                    case SDLK_SPACE:
                        m_UsePlunger = false;
                        m_ShootBall = true;
                        break;

                    // Flippers release
                    case SDLK_LEFT:
                        m_UseLeftFlipper = false;
                        break;

                    case SDLK_RIGHT:
                        m_UseRightFlipper = false;
                        break;

                    // Stop rotating table angle
                    //case SDLK_UP:
                    //    m_TableAngleUp = false;
                    //    break;

                    //case SDLK_DOWN:
                    //    m_TableAngleDown = false;
                    //    break;
                }
                break;
        }
    }
}

std::vector<RTRObject*> MainApp::CombineAllObjects() {
    std::vector<RTRObject*> allObjects;

    std::vector<RTRObject*> vector1 = m_RTRWorld->GetStaticCollidablePinballObjects();
    std::vector<RTRObject*> vector2 = m_RTRWorld->GetDynamicPinballObjects();
    allObjects.insert(std::end(allObjects), std::begin(vector1), std::end(vector1));
    allObjects.insert(std::end(allObjects), std::begin(vector2), std::end(vector2));

    return allObjects;
}

void MainApp::UpdateState(unsigned int td_milli)
{
    // Update directional camera to align with camera forward direction
    m_RTRWorld->GetLightingModel()->GetLight(0)->Direction = m_Camera->m_Front;

    // Setup Model and View matrices
    m_ModelMatrix = glm::mat4(1.0f);
    m_ViewMatrix = m_Camera->GetViewMatrix();

    // Reset collisions
    for (RTRSphere* dynaObject : m_RTRWorld->GetDynamicObjects()) {
        dynaObject->SetHasCollidedAABB(false);
        dynaObject->SetHasCollidedSphere(false);
    }

    // Clear grid
    m_RTRPhysicsEngine->ClearGrid();
    // Populate grid
    m_RTRPhysicsEngine->PopulateGrid();
    // Check for collisions
    m_RTRPhysicsEngine->UniformGridCollision();

    // Move Objects
    for (RTRSphere* dynaObject : m_RTRWorld->GetDynamicObjects()) {
        m_RTRPhysicsEngine->MoveBall(dynaObject, m_TimeDelta);
    }

    // Shoot current ball
    if (m_ShootBall) {
        for (RTRSphere* dynaObject : m_RTRWorld->GetDynamicObjects()) {
            if (!dynaObject->GetDidExit()) {

                float posZ = BALL_START_POS - dynaObject->GetPosition().z - 1.1f;
                m_RTRPhysicsEngine->TranslateBall(dynaObject, 0, 0, posZ);

                dynaObject->SetVerticalPower(-m_RTRPhysicsEngine->GetPower() + dynaObject->GetVerticalPower());
                dynaObject->SetHorizontalPower(m_RTRPhysicsEngine->GetPower() + dynaObject->GetHorizontalPower());
                dynaObject->SetMovingForward(true);
            }
        }
        m_ShootBall = false;
    }

    m_RTRWorld->PointLightColOn(td_milli);

    // Change angle of table
    //if (m_TableAngleUp) {
    //    m_RTRWorld->IncreaseTableAngle();
    //    m_RTRWorld->ChangeAllObjectsAngle();
    //}

    //if (m_TableAngleDown) {
    //    m_RTRWorld->DecreaseTableAngle();
    //    m_RTRWorld->ChangeAllObjectsAngle();
    //}
}

void MainApp::RenderFrame()
{
    m_RTRRenderer->SetUp();

    // Drawing skybox
    glm::mat4 skyboxView = glm::mat4(glm::mat3(m_Camera->GetViewMatrix()));
    m_RTRRenderer->RenderSkybox(skyboxView, m_ProjectionMatrix);
    m_RTRWorld->DrawSkybox();

    //For all static pinball objects
    for (RTRObject* staticPBObject : m_RTRWorld->GetStaticPinballObjects()) {
        m_RTRRenderer->RenderWithShaders(0, m_ViewMatrix, m_ProjectionMatrix,
            staticPBObject, m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
        if (m_DebugModeOn) m_RTRRenderer->RenderBoundingBoxes(4, m_ViewMatrix, m_ProjectionMatrix,
            staticPBObject->GetBoundingVolume(), m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
    }

    for (RTRObject* staticCPBObject : m_RTRWorld->GetStaticCollidablePinballObjects()) {
        if (staticCPBObject->GetName().find("Bar") != std::string::npos) {
            m_RTRRenderer->RenderEnvMapping(staticCPBObject, m_ViewMatrix, m_ProjectionMatrix, m_Camera);
        }
        else {
            m_RTRRenderer->RenderWithShaders(0, m_ViewMatrix, m_ProjectionMatrix,
                staticCPBObject, m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
        }
        if (m_DebugModeOn) m_RTRRenderer->RenderBoundingBoxes(4, m_ViewMatrix, m_ProjectionMatrix,
            staticCPBObject->GetBoundingVolume(), m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
    }

    //For all other dynamic objects, i.e: pinball ball
    for (RTRSphere* dynaObject : m_RTRWorld->GetDynamicObjects()) {
        if (!dynaObject->GetDestroyed()) {
            m_RTRRenderer->RenderWithShaders(2, m_ViewMatrix, m_ProjectionMatrix,
                dynaObject, m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
            if (m_DebugModeOn) m_RTRRenderer->RenderBoundingBoxes(4, m_ViewMatrix, m_ProjectionMatrix,
                dynaObject->GetBoundingVolume(), m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
        }
    }

    // For plunger use
    glm::mat4 plungerPosition = m_RTRWorld->GetDynamicPinballObjects().at(0)->GetTransformMatrix();
    glm::mat4 transformedPlungerPosition = m_RTRPhysicsEngine->UsePlunger(m_UsePlunger, m_TimeDelta, plungerPosition);

    m_RTRWorld->GetDynamicPinballObjects().at(0)->SetTransformMatrix(transformedPlungerPosition);
    m_RTRWorld->GetDynamicPinballObjects().at(0)->SetPosition(glm::vec3(transformedPlungerPosition[3]));
    m_RTRWorld->GetDynamicPinballObjects().at(0)->GetBoundingVolume()->SetTransformMatrix(transformedPlungerPosition);
    m_RTRWorld->GetDynamicPinballObjects().at(0)->GetBoundingVolume()->SetPosition(glm::vec3(transformedPlungerPosition[3]));

    if (!m_UsePlunger && !m_ShootBall) {
        m_RTRPhysicsEngine->ResetPower();
        if (m_RTRWorld->GetDynamicObjects().at(m_RTRWorld->GetCurrBall())->GetDidExit()) {
            m_RTRWorld->SetCurrBall(m_RTRWorld->GetCurrBall() + 1);
            m_RTRWorld->MakeNewBall(m_ModelMatrix);
        }
    }

    // Using flippers
    glm::mat4 transformedRightFlipper = m_RTRPhysicsEngine->UseRightFlipper(m_UseRightFlipper, 
        (RTRCube*)m_RTRWorld->GetDynamicPinballObjects().at(1));
    m_RTRWorld->GetDynamicPinballObjects().at(1)->SetTransformMatrix(transformedRightFlipper);
    m_RTRWorld->GetDynamicPinballObjects().at(1)->SetPosition(glm::vec3(transformedRightFlipper[3]));
    m_RTRWorld->GetDynamicPinballObjects().at(1)->GetBoundingVolume()->SetTransformMatrix(transformedRightFlipper);
    m_RTRWorld->GetDynamicPinballObjects().at(1)->GetBoundingVolume()->SetPosition(glm::vec3(transformedRightFlipper[3]));

    glm::mat4 transformedLeftFlipper = m_RTRPhysicsEngine->UseLeftFlipper(m_UseLeftFlipper, 
        (RTRCube*)m_RTRWorld->GetDynamicPinballObjects().at(2));
    m_RTRWorld->GetDynamicPinballObjects().at(2)->SetTransformMatrix(transformedLeftFlipper);
    m_RTRWorld->GetDynamicPinballObjects().at(2)->SetPosition(glm::vec3(transformedLeftFlipper[3]));
    m_RTRWorld->GetDynamicPinballObjects().at(2)->GetBoundingVolume()->SetTransformMatrix(transformedLeftFlipper);
    m_RTRWorld->GetDynamicPinballObjects().at(2)->GetBoundingVolume()->SetPosition(glm::vec3(transformedLeftFlipper[3]));

    for (RTRObject* dynaPBObject : m_RTRWorld->GetDynamicPinballObjects()) {
        m_RTRRenderer->RenderWithShaders(1, m_ViewMatrix, m_ProjectionMatrix,
            dynaPBObject, m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
        if (m_DebugModeOn) m_RTRRenderer->RenderBoundingBoxes(1, m_ViewMatrix, m_ProjectionMatrix,
            dynaPBObject->GetBoundingVolume(), m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
    }

    if (m_DebugModeOn) {
        m_RTRRenderer->DebugInfo(m_Console, m_FPS, m_Camera);
        // For drawing uniform grid
        for (RTRGrid* gridObject : m_RTRWorld->GetUniformGridObjects()) {
            m_RTRRenderer->RenderWithShaders(4, m_ViewMatrix, m_ProjectionMatrix,
                gridObject, m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
        }
        for (RTRObject* lightPos : m_RTRWorld->GetLightPositions()) {
            m_RTRRenderer->RenderWithShaders(4, m_ViewMatrix, m_ProjectionMatrix,
                lightPos, m_Camera, m_RTRWorld->GetLightingModel(), m_CurTime, m_TimeDelta);
        }
    }

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
