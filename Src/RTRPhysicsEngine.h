#pragma once
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRLighting.h"
#include "RTRObject.h"
#include "RTRWorld.h"
#include "Console.h"

enum class Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

typedef std::tuple<bool, Direction> Collision;

static const float DEFAULT_PLUNGER_Z_TRANS = 0.0f;
static const float GRAVITY = 9.18f;

class RTRPhysicsEngine {
public:
	RTRPhysicsEngine(RTRWorld* rtrworld);
	glm::mat4 UsePlunger(bool usingPlunger, float timer, glm::mat4 position);
	virtual float GetPower() { return power; };
	virtual void ResetPower() { power = 0.0f; };
	virtual void MoveBall(RTRSphere* sphere, float dt);
	virtual float DownwardsForce(RTRSphere* sphere, float dt);
	virtual void Collisions(RTRSphere* currBall, std::vector<RTRObject*> objects);
	virtual void CollisionsSpheres(RTRSphere* currBall, std::vector<RTRSphere*> spheres);
	virtual bool CheckCollisions_AABB_AABB(RTRObject* object1, RTRObject* object2);
	virtual bool CheckCollisions_AABB_Circle(RTRSphere* sphere, RTRObject* object);
	virtual Collision CheckCollisions_Circle_Circle(RTRSphere* sphere1, RTRSphere* sphere2);
	virtual std::vector<std::vector<glm::vec4>> SetUpUniformGrid();
	virtual Direction VectorDirection(glm::vec2 target);
	virtual void Done();

private:
	float plungerZTrans{ 0 };
	float power{ 0 };
	RTRWorld* m_RTRWorld{nullptr};
	std::vector<std::vector<glm::vec4>> m_UniformGrid2D;
	float m_tableAngle{ 0 };
};