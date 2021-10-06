#include "RTRPhysicsEngine.h"

RTRPhysicsEngine::RTRPhysicsEngine(RTRWorld* rtrworld)
{
	m_RTRWorld = rtrworld;
	plungerZTrans = DEFAULT_PLUNGER_Z_TRANS;
}

glm::mat4 RTRPhysicsEngine::UsePlunger(bool usingPlunger, float timer, glm::mat4 position) {
	glm::vec3 translation = glm::vec3(0, 0, 0);

	if (usingPlunger) {
		float increment = 0;
		if (plungerZTrans < 0.5f) {
			increment = (timer / 1000.0f);
			plungerZTrans += increment;
			power += 1.0f;
		}
		translation = glm::vec3(0, 0, increment);
	}
	else {
		power = 0;
		translation = glm::vec3(0, 0, -plungerZTrans);
		plungerZTrans = DEFAULT_PLUNGER_Z_TRANS;
	}

	position = glm::translate(position, translation);
	return position;
}

float RTRPhysicsEngine::GetPower()
{
	return power;
}

void RTRPhysicsEngine::ResetPower()
{
	power = 0.0f;
}

void RTRPhysicsEngine::Collisions() {

}

void RTRPhysicsEngine::Done() {
	plungerZTrans = 0;
	power = 0;
}