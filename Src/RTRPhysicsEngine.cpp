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
		//power = 0;
		translation = glm::vec3(0, 0, -plungerZTrans);
		plungerZTrans = DEFAULT_PLUNGER_Z_TRANS;
	}

	position = glm::translate(position, translation);
	return position;
}

void RTRPhysicsEngine::MoveBall(RTRSphere* sphere, float dt, float power)
{
	//glm::vec3 position = sphere->GetPosition();
	glm::mat4 transformMatrix = sphere->GetTransformMatrix();
	float zIncrement = 0;
	float yIncrement = 0;
	float xIncrement = 0;
	glm::vec3 position = sphere->GetPosition();

	if (sphere->GetMovingForward()) {
		//std::cout << "in check" << std::endl;
		zIncrement = -cos(UP_ANGLE) * (dt / 1000.0f) * (power);
		yIncrement = sin(UP_ANGLE) * (dt / 1000.0f) * (power);
		if (position.z <= -7.5f) {
			//glm::mat4 newPosition = sphere->GetTransformMatrix();
			//newPosition = glm::rotate(newPosition, 1.5708f, glm::vec3(0.0, 1.0, 0.0));
			//sphere->SetTransformMatrix(newPosition);
			sphere->SetMovingForward(false);
		}
	}
	else if (!sphere->GetMovingForward()) {
		//std::cout << "in not check" << std::endl;
		//xIncrement = -cos(UP_ANGLE) * (dt / 1000.0f) * (power);
		zIncrement = cos(UP_ANGLE) * (dt / 1000.0f) * (power);
		yIncrement = -sin(UP_ANGLE) * (dt / 1000.0f) * (power);
		if (position.z >= 7.5f) {
			sphere->SetMovingForward(true);
		}
	}

	glm::vec3 translation = glm::vec3(xIncrement, yIncrement, zIncrement);
	transformMatrix = glm::translate(transformMatrix, translation);
	sphere->SetTransformMatrix(transformMatrix);
	sphere->SetPosition(glm::vec3(transformMatrix[3]));
}

std::vector<std::vector<glm::vec4>> RTRPhysicsEngine::SetUpUniformGrid() {
	std::vector<std::vector<glm::vec4>> grid;
	std::vector<glm::vec3> gridPositions = m_RTRWorld->GetUniformGridPositions();

	int counter = 0;

	for (int j = 0; j <= 20; ++j) {
		std::vector<glm::vec4> newGrid;
		for (int i = 0; i <= 14; ++i) {
			glm::mat4 model = glm::mat4(0);

			float x = ((float)i / 14.0f * 14.0f) + -6.75f;
			float y = -2.25f;
			float z = ((float)j / 20.0f * 20.0f) + -9.75f;

			model = glm::rotate(model, DEFAULT_ANGLE, glm::vec3(1.0f, 0, 0));
			model = glm::translate(model, glm::vec3(x, y, z));

			glm::vec3 transformedPos = glm::vec3(model[3]);

			newGrid.push_back(glm::vec4(transformedPos, 0.5f));
		}
		grid.push_back(newGrid);
	}

	return grid;
}

void RTRPhysicsEngine::Collisions() {

}

void RTRPhysicsEngine::Done() {
	plungerZTrans = 0;
	power = 0;
}