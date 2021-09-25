//-----------------------------------------------------------------------------
// MainApp.h
//-----------------------------------------------------------------------------
// RMIT University, COSC1226: Real-Time Rendering and 3D Game Programming
//-----------------------------------------------------------------------------
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRLighting.h"
#include "RTRObject.h"
#include "Console.h"

#include <vector>
#include "RTRPhysicsEngine.h"
#include "RTRRenderer.h"

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
    RTRShader* m_PinballStaticShader{ nullptr };
    RTRShader* m_PinballDynamicShader{ nullptr };
    RTRShader* m_DynamicObjectsShader{ nullptr };
    
    //RTRShader* m_PlasmaShader{ nullptr };
    
    RTRCamera* m_Camera{ nullptr };
    RTRLightingModel* m_LightingModel{ nullptr };
    
    RTRObject* m_BottomPlane{ nullptr };
    RTRObject* m_TopBar{ nullptr };
    RTRObject* m_BottomBar{ nullptr };
    RTRObject* m_LeftBar{ nullptr };
    RTRObject* m_RightBar{ nullptr };
    RTRObject* m_SideShootBar{ nullptr };

    RTRObject* m_Plunger{ nullptr };

    RTRObject* m_Sphere{ nullptr };

    /*RTRObject* m_PlasmaCube{ nullptr };*/

    Console* m_Console{ nullptr };

    bool m_UsePlunger{ false };
    float lastTime{ 0 };
    float timer{ 0 };

    glm::mat4 m_ModelMatrix{ 1.0 };
    glm::mat4 m_ViewMatrix{ 1.0 };
    glm::mat4 m_ProjectionMatrix{ 1.0 };

    void CheckInput();
    void UpdateState(unsigned int td_milli);
    void RenderFrame(float timeDelta);

    std::vector<RTRShader*> ShaderVector;
    std::vector<RTRObject*> StaticPinballObjects;
    std::vector<RTRObject*> DynamicPinballObjects;
    std::vector<RTRObject*> DynamicObjects;
    RTRPhysicsEngine* m_RTRPhysicsEngine{ nullptr };
    RTRRenderer* m_RTRRenderer{ nullptr };
};
