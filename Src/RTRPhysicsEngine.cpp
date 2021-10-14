#include "RTRPhysicsEngine.h"

RTRPhysicsEngine::RTRPhysicsEngine(RTRWorld* rtrworld)
{
	m_RTRWorld = rtrworld;
	plungerZTrans = DEFAULT_PLUNGER_Z_TRANS;
	m_tableAngle = rtrworld->GetAngle();
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
	glm::mat4 transformMatrix = sphere->GetTransformMatrix();
	float zIncrement = 0;
	float yIncrement = 0;
	float xIncrement = 0;

	if (sphere->GetMovingForward()) {
		zIncrement += -(dt / 1000.0f) * (power);
		//yIncrement += sin(m_tableAngle) * (dt / 1000.0f) * (power);
	}
	if (sphere->GetMovingBackward()) {
		zIncrement += (dt / 1000.0f) * (power);
		//yIncrement += -sin(m_tableAngle) * (dt / 1000.0f) * (power);
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

	sphere->GetBoundingVolume()->SetTransformMatrix(transformMatrix);
	sphere->GetBoundingVolume()->SetPosition(glm::vec3(transformMatrix[3]));
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
	for (RTRObject* object : objects) {
		if (CheckCollisions_AABB_Circle(currBall, object)) {
			std::cout << object->GetName() << std::endl;

			if (object->GetName() == "m_TiltedBlock") {
				currBall->SetMovingForward(false);
				currBall->SetMovingBackward(true);
				currBall->SetMovingLeft(true);

				//std::cout << currBall->GetMovingLeft() << std::endl;
			}

			if (object->GetName() == "m_TopBar") {
				currBall->SetMovingForward(false);
				currBall->SetMovingBackward(true);
			}

			if (object->GetName() == "m_BottomBar" || object->GetName() == "m_Plunger") {
				currBall->SetMovingForward(true);
				currBall->SetMovingBackward(false);
			}

			if (object->GetName() == "m_LeftBar") {
				currBall->SetMovingLeft(false);
				currBall->SetMovingRight(true);
			}

			if (object->GetName() == "m_RightBar" || object->GetName() == "m_SideShootBar") {
				currBall->SetMovingLeft(true);
				currBall->SetMovingRight(false);
			}

			//if (currBall->GetVelocityDir().z < 0.0f) {
			//	currBall->SetMovingForward(false);
			//	currBall->SetMovingBackward(true);
			//}
			//if (currBall->GetVelocityDir().z > 0.0f) {
			//	currBall->SetMovingForward(true);
			//	currBall->SetMovingBackward(false);
			//}

			//if (currBall->GetVelocityDir().x < 0.0f) {
			//	currBall->SetMovingLeft(false);
			//	currBall->SetMovingRight(true);
			//}
			//if (currBall->GetVelocityDir().x > 0.0f) {
			//	currBall->SetMovingLeft(true);
			//	currBall->SetMovingRight(false);
			//}
		}
	}
}

void RTRPhysicsEngine::CollisionsSpheres(RTRSphere* currBall, std::vector<RTRSphere*> spheres) {
	for (RTRSphere* sphere : spheres) {
		if (sphere->GetName() != currBall->GetName()) {
			if (CheckCollisions_Circle_Circle(currBall, sphere)) {
				currBall->SetMovingForward(!currBall->GetMovingForward());
				currBall->SetMovingBackward(!currBall->GetMovingBackward());
				currBall->SetMovingLeft(!currBall->GetMovingLeft());
				currBall->SetMovingRight(!currBall->GetMovingRight());
			}
		}
	}
}

bool RTRPhysicsEngine::CheckCollisions_AABB_AABB(RTRObject* object1, RTRObject* object2) {
	// Check for collisions along x and z axis
	bool collXAxis = object1->GetPosition().x + (object1->GetScale().x / 2) >= object2->GetPosition().x
		&& object2->GetPosition().x + (object2->GetScale().x / 2) >= object1->GetPosition().x;

	bool collZAxis = object1->GetPosition().z + (object1->GetScale().z /2) >= object2->GetPosition().z
		&& object2->GetPosition().z + (object2->GetScale().z / 2) >= object1->GetPosition().z;

	return collXAxis && collZAxis;
}

bool RTRPhysicsEngine::CheckCollisions_AABB_Circle(RTRSphere* sphere, RTRObject* object) {
	float xMin, xMax, zMin, zMax = 0;

	xMin = object->GetPosition().x - (object->GetScale().x / 2);
	xMax = object->GetPosition().x + (object->GetScale().x / 2);
	zMin = object->GetPosition().z - (object->GetScale().z / 2);
	zMax = object->GetPosition().z + (object->GetScale().z / 2);

	float x = std::max(xMin, std::min(sphere->GetPosition().x, xMax));
	float z = std::max(zMin, std::min(sphere->GetPosition().z, zMax));

	float distance = std::sqrtf((x - sphere->GetPosition().x) * (x - sphere->GetPosition().x)
								+ (z - sphere->GetPosition().z) * (x - sphere->GetPosition().z));

	return distance < sphere->GetRadius() / 2;
}

bool RTRPhysicsEngine::CheckCollisions_Circle_Circle(RTRSphere* sphere1, RTRSphere* sphere2) {
	glm::vec2 center1(sphere1->GetPosition().x + sphere1->GetRadius(), sphere1->GetPosition().z + sphere1->GetRadius());
	glm::vec2 center2(sphere2->GetPosition().x + sphere2->GetRadius(), sphere2->GetPosition().z + sphere2->GetRadius());

	glm::vec2 difference = center1 - center2;

	return glm::length(difference) < sphere1->GetRadius();
}

float RTRPhysicsEngine::Clamp(float value, float min, float max) {
	return std::max(min, std::min(max, value));
}

void RTRPhysicsEngine::Done() {
	plungerZTrans = 0;
	power = 0;
}