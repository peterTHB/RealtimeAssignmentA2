#include "RTRPhysicsEngine.h"

RTRPhysicsEngine::RTRPhysicsEngine()
{
	plungerZTrans = 10.4f;
}

glm::vec3 RTRPhysicsEngine::UsePlunger(bool usingPlunger, float timer) {
	if (usingPlunger) {
		if (plungerZTrans < 11.4f) {
			plungerZTrans += (timer / 1000.0f);
		}
	}
	else {
		plungerZTrans = DEFAULT_PLUNGER_Z_TRANS;
	}

	glm::vec3 translation = glm::vec3(6.125f, -2.5f, plungerZTrans);
	return translation;
}