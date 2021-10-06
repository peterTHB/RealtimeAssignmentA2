#pragma once
#include "RTRLighting.h"
#include "RTRObject.h"

class RTRWorld {
public:
	RTRWorld(glm::mat4 modelMatrix);
    virtual void StartLighting();
    virtual void StartObjects(glm::mat4 modelMatrix);
    virtual void MakeNewBall(glm::mat4 modelMatrix);
    virtual unsigned int loadCubeMap(std::vector<std::string> faces);
    virtual void DrawSkybox();
    virtual void Done();
    RTRLightingModel* GetLightingModel() { return m_LightingModel; };
    std::vector<RTRObject*> GetStaticPinballObjects() { return StaticPinballObjects; };
    std::vector<RTRObject*> GetDynamicPinballObjects() { return DynamicPinballObjects; };
    std::vector<RTRObject*> GetDynamicObjects() { return DynamicObjects; };

    virtual unsigned int GetCubeMapTexture() { return cubemapTexture; };
    virtual void SetCubeMapTexture(unsigned int cubemap) { cubemapTexture = cubemap; };
    virtual unsigned int GetSkyboxVAO() { return skyboxVAO; };
    virtual void SetSkyboxVAO(unsigned int skybox) { skyboxVAO = skybox; };
    virtual std::vector<std::string> GetSkyboxFaces() { return skyboxFaces; };
    virtual void LoadSkyboxVAO();

    RTRLightingModel* m_LightingModel{ nullptr };

    std::vector<RTRObject*> StaticPinballObjects;
    std::vector<RTRObject*> DynamicPinballObjects;
    std::vector<RTRObject*> DynamicObjects;

    unsigned int cubemapTexture{ 0 };
    unsigned int skyboxVAO, skyboxVBO{ 0 };

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