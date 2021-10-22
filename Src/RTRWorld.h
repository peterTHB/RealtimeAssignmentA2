#pragma once
#include "RTRLighting.h"
#include "RTRObject.h"

static const float DEFAULT_ANGLE = 0.2f;

class RTRWorld {
public:
	RTRWorld(glm::mat4 modelMatrix);
    virtual void StartLighting();
    virtual void StartObjects(glm::mat4 modelMatrix);
    virtual void IncreaseTableAngle() { m_TableAngle += 0.001f; };
    virtual void DecreaseTableAngle() { m_TableAngle -= 0.001f; };
    virtual void MakeNewBall(glm::mat4 modelMatrix);
    virtual unsigned int loadCubeMap(std::vector<std::string> faces);
    virtual void SetTableAngle(float angle) { m_TableAngle = angle; };
    virtual float GetTableAngle() { return m_TableAngle; };
    virtual void ChangeAllObjectsAngle();
    virtual void SetCurrBall(int num) { m_CurrBall = num; };
    virtual int GetCurrBall() { return m_CurrBall; };
    virtual void DrawSkybox();
    virtual void MakeUniformGrid(float xOffset, float zOffset, int horizontal, int vertical, glm::mat4 modelMatrix);
    virtual std::vector<glm::vec3> MakeGridPositions(float xOffset, float zOffset, int horizontal, int vertical);
    virtual void Done();
    RTRLightingModel* GetLightingModel() { return m_LightingModel; };
    std::vector<RTRObject*> GetStaticPinballObjects() { return StaticPinballObjects; };
    std::vector<RTRObject*> GetStaticCollidablePinballObjects() { return StaticCollidablePinballObjects; };
    std::vector<RTRObject*> GetDynamicPinballObjects() { return DynamicPinballObjects; };
    std::vector<RTRSphere*> GetDynamicObjects() { return DynamicObjects; };
    std::vector<RTRGrid*> GetUniformGridObjects() { return UniformGridObjects; };
    std::vector<glm::vec3> GetUniformGridPositions() { return UniformGridPositions; };

    virtual unsigned int LoadTexture(std::string textureFile);
    virtual unsigned int GetCubeMapTexture() { return cubemapTexture; };
    virtual void SetCubeMapTexture(unsigned int cubemap) { cubemapTexture = cubemap; };
    virtual unsigned int GetSkyboxVAO() { return skyboxVAO; };
    virtual void SetSkyboxVAO(unsigned int skybox) { skyboxVAO = skybox; };
    virtual std::vector<std::string> GetSkyboxFaces() { return skyboxFaces; };
    virtual void LoadSkyboxVAO();

    RTRLightingModel* m_LightingModel{ nullptr };

    std::vector<RTRObject*> StaticPinballObjects;
    std::vector<RTRObject*> StaticCollidablePinballObjects;
    std::vector<RTRObject*> DynamicPinballObjects;
    std::vector<RTRSphere*> DynamicObjects;
    std::vector<RTRGrid*> UniformGridObjects;
    std::vector<glm::vec3> UniformGridPositions;

    unsigned int m_SmoothWoodTexture{ 0 };
    unsigned int m_DarkWoodTexture{ 0 };
    unsigned int m_FloorWoodTexture{ 0 };
    unsigned int m_ShinyMetalTexture{ 0 };
    unsigned int m_RoughMetalTexture{ 0 };
    unsigned int m_OuterRimTexture{ 0 };

    unsigned int cubemapTexture{ 0 };
    unsigned int skyboxVAO, skyboxVBO{ 0 };

    int m_CurrBall{ 0 };
    float m_TableAngle{ DEFAULT_ANGLE };

    std::vector<std::string> skyboxFaces{
        "Src/Skybox/rightSpace.jpg",
        "Src/Skybox/leftSpace.jpg",
        "Src/Skybox/topSpace.jpg",
        "Src/Skybox/bottomSpace.jpg",
        "Src/Skybox/frontSpace.jpg",
        "Src/Skybox/backSpace.jpg"
    };

    std::vector<float> skyboxVertices{
        // positions          
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,

        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f
    };
};