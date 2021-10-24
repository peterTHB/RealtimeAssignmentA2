//-----------------------------------------------------------------------------
// MainApp.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRWorld.h"
#include "Console.h"
#include "RTRPhysicsEngine.h"
#include "RTRRenderer.h"
#include "RTRParticles.h"

static const float BALL_START_POS = 6.66761f;

class MainApp : public RTRApp
{
public:
    MainApp(const char* title, bool fullscreen = false, int width = 1024, int height = 768)
        : RTRApp(title, fullscreen, width, height) 
    { }
    int Init();
    void Done();
    bool Tick();

private:
    bool m_QuitApp{ false };
    
    RTRWorld* m_RTRWorld{ nullptr };
    RTRCamera* m_Camera{ nullptr };

    Console* m_Console{ nullptr };

    bool m_ShootBall{ false };
    int m_NumBallsShot{ 0 };
    bool m_UsePlunger{ false };
    bool m_UseRightFlipper{ false };
    bool m_UseLeftFlipper{ false };
    bool m_TableAngleUp{ false };
    bool m_TableAngleDown{ false };
    bool m_DebugModeOn{ false };

    glm::mat4 m_ModelMatrix{ 1.0 };
    glm::mat4 m_ViewMatrix{ 1.0 };
    glm::mat4 m_ProjectionMatrix{ 1.0 };

    void CheckInput();
    void UpdateState(unsigned int td_milli);
    void RenderFrame();

    RTRPhysicsEngine* m_RTRPhysicsEngine{ nullptr };
    RTRRenderer* m_RTRRenderer{ nullptr };
    RTRParticles* m_RTRParticles{ nullptr };
};
