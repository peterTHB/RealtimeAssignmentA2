#pragma once
#include "RTRLighting.h"
#include "RTRObject.h"

std::vector<std::string> skyboxFaces{
    "Src/rightSpace.gif",
    "Src/leftSpace.gif",
    "Src/topSpace.gif",
    "Src/bottomSpace.gif",
    "Src/frontSpace.gif",
    "Src/backSpace.gif"
};

class RTRWorld {
public:
	RTRWorld();
    virtual void StartLighting();
    virtual void StartObjects();
    virtual void MakeNewBall();
    virtual unsigned int loadCubeMap(std::vector<std::string> faces);
    virtual void Done();
    RTRLightingModel* GetLightingModel() { return m_LightingModel; };
    std::vector<RTRObject*> GetStaticPinballObjects() { return StaticPinballObjects; };
    std::vector<RTRObject*> GetDynamicPinballObjects() { return DynamicPinballObjects; };
    std::vector<RTRObject*> GetDynamicObjects() { return DynamicObjects; };

    RTRLightingModel* m_LightingModel{ nullptr };

    std::vector<RTRObject*> StaticPinballObjects;
    std::vector<RTRObject*> DynamicPinballObjects;
    std::vector<RTRObject*> DynamicObjects;

    unsigned int cubemapTexture{ 0 };

private:

};