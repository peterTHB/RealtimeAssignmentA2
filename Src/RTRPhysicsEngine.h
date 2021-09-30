#pragma once
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRLighting.h"
#include "RTRObject.h"
#include "Console.h"

static const float DEFAULT_PLUNGER_Z_TRANS = 10.4f;

class RTRPhysicsEngine {
public:
	RTRPhysicsEngine();
	glm::vec3 UsePlunger(bool usingPlunger, float timer);
	virtual float GetPower();
	virtual void ResetPower();
	virtual void Done();

private:
	float plungerZTrans{ 0 };
	float power{ 0 };
};