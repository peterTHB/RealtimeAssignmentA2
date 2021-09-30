#pragma once
#include "RTRLighting.h"
#include "RTRObject.h"


class RTRWorld {
public:
	RTRWorld();
    virtual void StartLighting();
    virtual void StartObjects();
    virtual void MakeNewBall();
    virtual void Done();
    RTRLightingModel* GetLightingModel() { return m_LightingModel; };
    std::vector<RTRObject*> GetStaticPinballObjects() { return StaticPinballObjects; };
    std::vector<RTRObject*> GetDynamicPinballObjects() { return DynamicPinballObjects; };
    std::vector<RTRObject*> GetDynamicObjects() { return DynamicObjects; };

    RTRLightingModel* m_LightingModel{ nullptr };

    std::vector<RTRObject*> StaticPinballObjects;
    std::vector<RTRObject*> DynamicPinballObjects;
    std::vector<RTRObject*> DynamicObjects;

private:

};