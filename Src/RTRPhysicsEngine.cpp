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

void RTRPhysicsEngine::MoveBall(RTRSphere* sphere, float dt)
{
	glm::mat4 transformMatrix = sphere->GetTransformMatrix();
	float zIncrement = 0;
	float yIncrement = 0;
	float xIncrement = 0;

	//std::cout << "---------------------" << std::endl;
	//std::cout << sphere->GetName() << std::endl;
	//std::cout << "Can Move: " << sphere->GetCanMove() << std::endl;
	//std::cout << "Moving Forward: " << sphere->GetMovingForward() << std::endl;
	//std::cout << "Moving Left: " << sphere->GetMovingLeft() << std::endl;
	//std::cout << "Moving Right: " << sphere->GetMovingRight() << std::endl;
	//std::cout << "---------------------" << std::endl;

	if (sphere->GetCanMove()) zIncrement += DownwardsForce(sphere, dt);

	if (sphere->GetMovingForward() && sphere->GetCanMove()) {
		sphere->SetVerticalPower(sphere->GetVerticalPower() + (dt / 1000.0f * GRAVITY));
		zIncrement += (dt / 1000.0f) * sphere->GetVerticalPower();
	}

	if (sphere->GetMovingRight() || sphere->GetMovingLeft()) {
		sphere->SetHorizontalPower(sphere->GetHorizontalPower() - (dt / 1000.0f * GRAVITY / 10));
		if (sphere->GetHorizontalPower() <= 0.0f) {
			sphere->SetHorizontalPower(0.0f);
			sphere->SetCanMove(false);
		}
	}

	if (sphere->GetMovingRight()) xIncrement += (dt / 1000.0f) * GRAVITY / 10 * fabs(sphere->GetHorizontalPower());
	else if (sphere->GetMovingLeft()) xIncrement -= (dt / 1000.0f) * GRAVITY / 10 * fabs(sphere->GetHorizontalPower());

	glm::vec3 translation = glm::vec3(xIncrement, yIncrement, zIncrement);
	transformMatrix = glm::translate(transformMatrix, translation);
	sphere->SetTransformMatrix(transformMatrix);
	sphere->SetPosition(glm::vec3(transformMatrix[3]));

	sphere->GetBoundingVolume()->SetTransformMatrix(transformMatrix);
	sphere->GetBoundingVolume()->SetPosition(glm::vec3(transformMatrix[3]));
}

float RTRPhysicsEngine::DownwardsForce(RTRSphere* sphere, float dt) {
	float force = 0;

	force = (dt / 1000.0f) * GRAVITY / 2 * DEFAULT_ANGLE;

	return force;
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
	bool hasCollided = false;

	for (RTRObject* object : objects) {

		bool collided = CheckCollisions_AABB_Circle(currBall, object);

		if (collided) {
			hasCollided = true;
			currBall->SetVerticalPower(currBall->GetVerticalPower() * 0.9f);

			std::cout << "Still colliding" << std::endl;

			//std::cout << object->GetName() << std::endl;

			if (object->GetName() == "m_TiltedBlock") {
				currBall->SetVerticalPower(fabs(currBall->GetVerticalPower()));
				currBall->SetMovingForward(true);
				currBall->SetMovingLeft(true);
				currBall->SetDidExit(true);
			}

			if (object->GetName() == "m_TopBar") {
				currBall->SetVerticalPower(fabs(currBall->GetVerticalPower()));
			}
			else if (object->GetName() == "m_BottomBar" || object->GetName() == "m_Plunger") {
				currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
				if (currBall->GetVerticalPower() >= -0.05f && currBall->GetVerticalPower() <= 0.05f) {
					currBall->SetMovingForward(false);
					currBall->SetCanMove(false);
				}
			}
			if (object->GetName() == "m_LeftBar") {
				currBall->SetMovingLeft(false);
				currBall->SetMovingRight(true);
				currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.9f - GRAVITY / 10);
			}
			else if (object->GetName() == "m_RightBar" || object->GetName() == "m_SideShootBar") {
				currBall->SetMovingLeft(true);
				currBall->SetMovingRight(false);
				currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.9f - GRAVITY / 10);
			}
		}

		//Collision collided = CheckCollisions_AABB_Circle(currBall, object);

		//if (std::get<0>(collided)) {
		//	Direction dir = std::get<1>(collided);

		//	currBall->SetPower(currBall->GetPower() * 0.9f);

		//	//std::cout << object->GetName() << std::endl;

		//	if (dir == Direction::UP) {
		//		std::cout << currBall->GetName() << " Collided with " << object->GetName() << " in UP" << std::endl;
		//		currBall->SetPower(fabs(currBall->GetPower()));
		//		if (object->GetName() == "m_TiltedBlock") {
		//			currBall->SetMovingForward(true);
		//			currBall->SetMovingLeft(true);
		//			currBall->SetDidExit(true);
		//		}
		//	}
		//	else if (dir == Direction::DOWN) {
		//		std::cout << currBall->GetName() << " Collided with " << object->GetName() << " in DOWN" << std::endl;
		//		currBall->SetPower(-fabs(currBall->GetPower()));

		//		if (currBall->GetPower() >= -0.1f && currBall->GetPower() <= 0.1f) {
		//			currBall->SetMovingForward(false);
		//			currBall->SetCanMove(false);
		//		}
		//	}
		//	if (dir == Direction::LEFT) {
		//		std::cout << currBall->GetName() << " Collided with " << object->GetName() << " in LEFT" << std::endl;
		//		currBall->SetMovingLeft(false);
		//		currBall->SetMovingRight(true);
		//	}
		//	else if (dir == Direction::RIGHT) {
		//		std::cout << currBall->GetName() << " Collided with " << object->GetName() << " in RIGHT" << std::endl;
		//		currBall->SetMovingLeft(true);
		//		currBall->SetMovingRight(false);
		//	}
		//}
		//else {
		//	currBall->SetCanMove(true);
		//}
	}

	if (!hasCollided) {
		//std::cout << currBall->GetName() << " Not colliding" << std::endl;
		currBall->SetCanMove(true);
	}
}

void RTRPhysicsEngine::CollisionsSpheres(RTRSphere* currBall, std::vector<RTRSphere*> spheres) {
	for (RTRSphere* sphere : spheres) {
		if (sphere->GetName() != currBall->GetName()) {

			Collision collided = CheckCollisions_Circle_Circle(currBall, sphere);

			if (std::get<0>(collided)) {
				Direction dir = std::get<1>(collided);

				currBall->SetVerticalPower(currBall->GetVerticalPower() * 0.9f);

				if (dir == Direction::UP) {
					currBall->SetVerticalPower(fabs(currBall->GetVerticalPower()));
					//currBall->SetMovingForward(false);
					//currBall->SetMovingBackward(true);
				}
				else if (dir == Direction::DOWN) {
					currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
					//currBall->SetMovingForward(true);
					//currBall->SetMovingBackward(false);

					if (currBall->GetVerticalPower() >= -0.1f && currBall->GetVerticalPower() <= 0.1f) {
						currBall->SetMovingForward(false);
						currBall->SetCanMove(false);
					}
				}
				if (dir == Direction::LEFT) {
					currBall->SetMovingLeft(false);
					currBall->SetMovingRight(true);
				}
				else if (dir == Direction::RIGHT) {
					currBall->SetMovingLeft(true);
					currBall->SetMovingRight(false);
				}
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
								+ (z - sphere->GetPosition().z) * (z - sphere->GetPosition().z));

	return distance < sphere->GetRadius();

	//if (distance < sphere->GetRadius()) {
	//	return std::make_tuple(true, VectorDirection(glm::vec2(x, z)));
	//}
	//else {
	//	return std::make_tuple(false, Direction::UP);
	//}
}

Collision RTRPhysicsEngine::CheckCollisions_Circle_Circle(RTRSphere* sphere1, RTRSphere* sphere2) {
	glm::vec2 center1(sphere1->GetPosition().x + sphere1->GetRadius(), sphere1->GetPosition().z + sphere1->GetRadius());
	glm::vec2 center2(sphere2->GetPosition().x + sphere2->GetRadius(), sphere2->GetPosition().z + sphere2->GetRadius());

	glm::vec2 difference = center1 - center2;

	if (glm::length(difference) < sphere1->GetRadius()) {
		return std::make_tuple(true, VectorDirection(difference));
	}
	else {
		return std::make_tuple(false, Direction::UP);
	}
}

Direction RTRPhysicsEngine::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, -1.0f),	// up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, 1.0f),	// down
		glm::vec2(-1.0f, 0.0f)	// left
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

void RTRPhysicsEngine::Done() {
	plungerZTrans = 0;
	power = 0;
}