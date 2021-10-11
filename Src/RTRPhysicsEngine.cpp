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
	//glm::vec3 position = sphere->GetPosition();

	//if (position.z <= -8.5f) {
	//	sphere->SetMovingForward(false);
	//	sphere->SetMovingBackward(true);
	//}
	//if (position.z >= 8.5f) {
	//	sphere->SetMovingBackward(false);
	//	sphere->SetMovingForward(true);
	//}
	//if (position.x >= 6.5f) {
	//	sphere->SetMovingRight(false);
	//	sphere->SetMovingLeft(true);
	//}
	//if (position.x <= -6.5f) {
	//	sphere->SetMovingLeft(false);
	//	sphere->SetMovingRight(true);
	//}

	if (sphere->GetMovingForward()) {
		zIncrement += -cos(UP_ANGLE) * (dt / 1000.0f) * (power);
		yIncrement += sin(UP_ANGLE) * (dt / 1000.0f) * (power);
	}
	if (sphere->GetMovingBackward()) {
		zIncrement += cos(UP_ANGLE) * (dt / 1000.0f) * (power);
		yIncrement += -sin(UP_ANGLE) * (dt / 1000.0f) * (power);
	}
	if (sphere->GetMovingRight()) {
		xIncrement += (dt / 1000.0f) * (power);
	}
	if (sphere->GetMovingLeft()) {
		xIncrement += -(dt / 1000.0f) * (power);
	}

	glm::vec3 translation = glm::vec3(xIncrement, yIncrement, zIncrement);
	sphere->SetVelocityDir(translation);
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

void RTRPhysicsEngine::Collisions(RTRSphere* currBall, std::vector<RTRObject*> objects) {
	glm::vec3 velocityDir = currBall->GetVelocityDir();

	for (RTRObject* object : objects) {
		if (CheckCollisions(currBall, object)) {
			if (object->GetName() == "m_TopBar" || object->GetName() == "m_Plunger") {
				std::cout << currBall->GetName() << std::endl;
				std::cout << currBall->GetPosition().x << std::endl;
				std::cout << currBall->GetPosition().z << std::endl;
				std::cout << currBall->GetScale().x << std::endl;
				std::cout << currBall->GetScale().z << std::endl;

				std::cout << object->GetName() << std::endl;
				std::cout << object->GetPosition().x << std::endl;
				std::cout << object->GetPosition().z << std::endl;
				std::cout << object->GetScale().x << std::endl;
				std::cout << object->GetScale().z << std::endl;

				if (velocityDir.z < 0.0f) {
					currBall->SetMovingForward(false);
					currBall->SetMovingBackward(true);
				}
				else if (velocityDir.z > 0.0f) {
					currBall->SetMovingForward(true);
					currBall->SetMovingBackward(false);
				}

				if (velocityDir.x < 0.0f) {
					currBall->SetMovingLeft(false);
					currBall->SetMovingRight(true);
				}
				else if (velocityDir.x > 0.0f) {
					currBall->SetMovingLeft(true);
					currBall->SetMovingRight(false);
				}
			}

			//currBall->SetMovingForward(!currBall->GetMovingForward());
			//currBall->SetMovingBackward(!currBall->GetMovingBackward());
			//currBall->SetMovingLeft(!currBall->GetMovingLeft());
			//currBall->SetMovingRight(!currBall->GetMovingRight());
		}
	}
}

bool RTRPhysicsEngine::CheckCollisions(RTRObject* object1, RTRObject* object2) {
	// Check for collisions along x and z axis

	bool collXAxis = object1->GetPosition().x + object1->GetScale().x >= object2->GetPosition().x
		&& object2->GetPosition().x + object2->GetScale().x >= object1->GetPosition().x;

	bool collZAxis = object1->GetPosition().z + object1->GetScale().z >= object2->GetPosition().z
		&& object2->GetPosition().z + object2->GetScale().z >= object1->GetPosition().z;

	return collXAxis && collZAxis;
}

void RTRPhysicsEngine::Done() {
	plungerZTrans = 0;
	power = 0;
}