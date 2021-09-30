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

    // Setup projection matrix and viewport transform. 
    // These won't need to change as we're not worrying about screen size changes for this assignment
    m_ProjectionMatrix = glm::perspective(glm::radians(60.0f), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
    glViewport(0, 0, m_WindowWidth, m_WindowHeight);

    // Create and initialise camera
    m_Camera = new RTRCamera(glm::vec3(0.0, 11.0, 13.0), glm::vec3(0.0, 1.0, 0.0));

    // Create and initialise lighting model
    m_LightingModel = new RTRLightingModel();

    m_RTRPhysicsEngine = new RTRPhysicsEngine();
    m_RTRRenderer = new RTRRenderer();
    
    // Add directional light to follow camera view direction
    m_LightingModel->AddLight({
        .Type = RTRLightType::DirectionalLight,
        .Ambient = glm::vec3(0.2, 0.2, 0.2),
        .Diffuse = glm::vec3(1.0, 1.0, 1.0),
        .Specular = glm::vec3(0.5, 0.5, 0.5),
        .Direction = glm::vec3(1.0, 0.0, 0.0)
        });
    // Add x-axis red spot lights (palce all at x+3.0 so they are positioned around shaded cube)
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(1.0, 0.0, 0.0),
        .Diffuse = glm::vec3(1.0, 0.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(2.0 + 3.0, 0.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
    });
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(1.0, 0.0, 0.0),
        .Diffuse = glm::vec3(1.0, 0.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(-2.0 + 3.0, 0.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Add y-axis green spot lights
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.2, 0.0),
        .Diffuse = glm::vec3(0.0, 1.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0 + 3.0, 2.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.2, 0.0),
        .Diffuse = glm::vec3(0.0, 1.0, 0.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0 + 3.0, -2.0, 0.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    // Add z-axis blue spot lights
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.0, 0.2),
        .Diffuse = glm::vec3(0.0, 0.0, 1.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0 + 3.0, 0.0, 2.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });
    m_LightingModel->AddLight({
        .Type = RTRLightType::PointLight,
        .Ambient = glm::vec3(0.0, 0.0, 0.2),
        .Diffuse = glm::vec3(0.0, 0.0, 1.0),
        .Specular = glm::vec3(1.0, 1.0, 1.0),
        .Position = glm::vec3(0.0 + 3.0, 0.0, -2.0),
        .Constant = 1.0f,
        .Linear = 0.35f,
        .Quadratic = 0.44f
        });

    // Create two cube objects
    // You might want to maintain a vector of objects for your assignment
    // Static Pinball machine and related objects
    m_BottomPlane = new RTRCube();
    m_BottomPlane->Init();

    m_TopBar = new RTRCube();
    m_TopBar->Init();

    m_BottomBar = new RTRCube();
    m_BottomBar->Init();

    m_LeftBar = new RTRCube();
    m_LeftBar->Init();

    m_RightBar = new RTRCube();
    m_RightBar->Init();

    m_SideShootBar = new RTRCube();
    m_SideShootBar->Init();

    StaticPinballObjects.push_back(m_BottomPlane);
    StaticPinballObjects.push_back(m_TopBar);
    StaticPinballObjects.push_back(m_BottomBar);
    StaticPinballObjects.push_back(m_LeftBar);
    StaticPinballObjects.push_back(m_RightBar);
    StaticPinballObjects.push_back(m_SideShootBar);

    // Dynamic pinball objects
    m_Plunger = new RTRCube();
    m_Plunger->Init();
    RTRMaterial_t plungerMat = { {0.1, 0.8, 0.1 }, { 0.4, 0.4, 0.4 }, { 0.7, 0.7, 0.7 }, 64.0 };
    m_Plunger->SetMaterial(plungerMat);
    m_Plunger->SetPosition(m_RTRPhysicsEngine->UsePlunger(m_UsePlunger, m_TimeDelta));

    DynamicPinballObjects.push_back(m_Plunger);

    // Test sphere
    m_Sphere = new RTRSphere();
    m_Sphere->Init();
    RTRMaterial_t sphereMat = { {0.1, 0.1, 0.8 }, { 0.6, 0.6, 0.6 }, { 0.8, 0.8, 0.8 }, 64.0 };
    m_Sphere->SetMaterial(sphereMat);
    m_Sphere->SetPosition(glm::vec3(6.125f, -2.5f, 8.5f));

    DynamicObjects.push_back(m_Sphere);

    // Create and initialise the debug console/overlay
    m_Console = new Console();
    m_Console->Init();

    return 0;
}

void MainApp::Done()
{
    m_BottomPlane->End(); delete m_BottomPlane;
    m_TopBar->End(); delete m_TopBar;
    m_BottomBar->End(); delete m_BottomBar;
    m_LeftBar->End(); delete m_LeftBar;
    m_RightBar->End(); delete m_RightBar;
    m_SideShootBar->End(); delete m_SideShootBar;
    
    m_Plunger->End(); delete m_Plunger;

    m_Sphere->End(); delete m_Sphere;

    m_Console->End(); delete m_Console;
    
    StaticPinballObjects.clear();
    DynamicPinballObjects.clear();
    DynamicObjects.clear();

    m_RTRRenderer->Done();
    delete m_RTRRenderer;
    
    m_RTRPhysicsEngine->Done();
    delete m_RTRPhysicsEngine;

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
                        break;

                }
                break;
        }
    }
}

void MainApp::UpdateState(unsigned int td_milli)
{
    // Update directional camera to align with camera forward direction
    m_LightingModel->GetLight(0)->Direction = m_Camera->m_Front;

    // Setup Model and View matrices
    m_ModelMatrix = glm::mat4(1.0f);
    m_ViewMatrix = m_Camera->GetViewMatrix();
}

void MainApp::RenderFrame()
{
    m_RTRRenderer->SetUp();

    // Bottom Plane
    glm::vec3 bottomPlaneTrans = glm::vec3(0.0f, -3.0f, 0.0f);
    glm::vec3 bottomPlaneScale = glm::vec3(7.0f, 0.25f, 10.0f);
    glm::vec3 bottomPlaneRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    m_RTRRenderer->RenderWithShaders(0, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_BottomPlane, m_Camera, m_LightingModel, m_CurTime, m_TimeDelta, bottomPlaneTrans, 
        bottomPlaneScale, bottomPlaneRotate);

    // Top Bar
    glm::vec3 topBarTrans = glm::vec3(0.0f, -1.75f, -9.75f);
    glm::vec3 topBarScale = glm::vec3(7.0f, 1.0f, 0.25f);
    glm::vec3 topBarRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    m_RTRRenderer->RenderWithShaders(0, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_TopBar, m_Camera, m_LightingModel, m_CurTime, m_TimeDelta, topBarTrans,
        topBarScale, topBarRotate);

    // Bottom Bar
    glm::vec3 bottomBarTrans = glm::vec3(0.0f, -1.75f, 9.75f);
    glm::vec3 bottomBarScale = glm::vec3(7.0f, 1.0f, 0.25f);
    glm::vec3 bottomBarRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    m_RTRRenderer->RenderWithShaders(0, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_TopBar, m_Camera, m_LightingModel, m_CurTime, m_TimeDelta, bottomBarTrans,
        bottomBarScale, bottomBarRotate);

    // Left Bar
    glm::vec3 leftBarTrans = glm::vec3(-6.75f, -1.75f, 0.0f);
    glm::vec3 leftBarScale = glm::vec3(0.25f, 1.0f, 9.5f);
    glm::vec3 leftBarRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    m_RTRRenderer->RenderWithShaders(0, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_TopBar, m_Camera, m_LightingModel, m_CurTime, m_TimeDelta, leftBarTrans,
        leftBarScale, leftBarRotate);

    // Right Bar
    glm::vec3 rightBarTrans = glm::vec3(6.75f, -1.75f, 0.0f);
    glm::vec3 rightBarScale = glm::vec3(0.25f, 1.0f, 9.5f);
    glm::vec3 rightBarRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    m_RTRRenderer->RenderWithShaders(0, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_TopBar, m_Camera, m_LightingModel, m_CurTime, m_TimeDelta, rightBarTrans,
        rightBarScale, rightBarRotate);

    // Side shoot barrier
    glm::vec3 sideShootBarTrans = glm::vec3(5.5f, -1.75f, 1.5f);
    glm::vec3 sideShootBarScale = glm::vec3(0.25f, 1.0f, 8.0f);
    glm::vec3 sideShootBarRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    m_RTRRenderer->RenderWithShaders(0, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_TopBar, m_Camera, m_LightingModel, m_CurTime, m_TimeDelta, sideShootBarTrans,
        sideShootBarScale, sideShootBarRotate);

    // Pinball Plunger
    m_Plunger->SetPosition(m_RTRPhysicsEngine->UsePlunger(m_UsePlunger, m_TimeDelta));
    glm::vec3 plungerTrans = m_Plunger->GetPosition();
    glm::vec3 plungerScale = glm::vec3(0.25f, 0.25f, 1.5f);
    glm::vec3 plungerRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    m_RTRRenderer->RenderWithShaders(1, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_Plunger, m_Camera, m_LightingModel, m_CurTime, m_TimeDelta, plungerTrans,
        plungerScale, plungerRotate);

    if (!m_UsePlunger) m_RTRPhysicsEngine->ResetPower();
    
    // Sphere test
    glm::vec3 sphereTrans = m_Sphere->GetPosition();
    glm::vec3 sphereScale = glm::vec3(0.3f, 0.3f, 0.3f);
    glm::vec3 sphereRotate = glm::vec3(1.0f, 0.0f, 0.0f);
    m_RTRRenderer->RenderWithShaders(2, m_ModelMatrix, m_ViewMatrix, m_ProjectionMatrix,
        m_Sphere, m_Camera, m_LightingModel, m_CurTime, m_TimeDelta, sphereTrans,
        sphereScale, sphereRotate);

    m_RTRRenderer->DebugInfo(m_Console, m_FPS, m_Camera);

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
