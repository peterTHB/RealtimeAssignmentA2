#pragma once
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRLighting.h"
#include "RTRObject.h"
#include "RTRWorld.h"
#include "Console.h"

static const float DEFAULT_PLUNGER_Z_TRANS = 0.0f;
static const float GRAVITY = -9.18f;
static const float UP_ANGLE = 0.22f;
static const float DOWN_ANGLE = 3.14159f;

class RTRPhysicsEngine {
public:
	RTRPhysicsEngine(RTRWorld* rtrworld);
	glm::mat4 UsePlunger(bool usingPlunger, float timer, glm::mat4 position);
	virtual float GetPower() { return power; };
	virtual void ResetPower() { power = 0.0f; };
	virtual void MoveBall(RTRSphere* sphere, float dt, float power);
	virtual void Collisions(RTRSphere* currBall, std::vector<RTRObject*> objects);
	virtual bool CheckCollisions(RTRObject* object1, RTRObject* object2);
	virtual std::vector<std::vector<glm::vec4>> SetUpUniformGrid();
	virtual void Done();

private:
	float plungerZTrans{ 0 };
	float power{ 0 };
	RTRWorld* m_RTRWorld{nullptr};
	std::vector<std::vector<glm::vec4>> m_UniformGrid2D;
};