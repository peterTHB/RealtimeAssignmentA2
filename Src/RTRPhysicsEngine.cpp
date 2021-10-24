#include "RTRPhysicsEngine.h"

RTRPhysicsEngine::RTRPhysicsEngine(RTRWorld* rtrworld)
{
	m_RTRWorld = rtrworld;
	plungerZTrans = DEFAULT_PLUNGER_Z_TRANS;
	m_GridRow = m_RTRWorld->GetGridRow();
	m_GridColumn = m_RTRWorld->GetGridColumn();
	SetUpUniformGrid();
	PopulateGrid();
}

glm::mat4 RTRPhysicsEngine::UsePlunger(bool usingPlunger, float timer, glm::mat4 position) {
	glm::vec3 translation = glm::vec3(0, 0, 0);

	if (usingPlunger) {
		float increment = 0;
		if (plungerZTrans < 0.5f) {
			increment = (timer / 1000.0f);
			plungerZTrans += increment;
			power += 1.1f;
		}
		translation = glm::vec3(0, 0, increment);
	}
	else {
		translation = glm::vec3(0, 0, -plungerZTrans);
		plungerZTrans = DEFAULT_PLUNGER_Z_TRANS;
	}

	position = glm::translate(position, translation);
	return position;
}

glm::mat4 RTRPhysicsEngine::UseRightFlipper(bool usingFlipper, RTRCube* flipper) {
	glm::mat4 currPosition = glm::mat4(1.0f);

	currPosition = glm::rotate(currPosition, m_RTRWorld->GetTableAngle(), glm::vec3(1.0f, 0.0f, 0.0f));
	currPosition = glm::translate(currPosition, glm::vec3(0.8f, -0.75f, 7.85f));
	currPosition = glm::rotate(currPosition, (float)(195 * M_PI / 180), glm::vec3(0.0f, 1.0f, 0.0f));
	currPosition = glm::scale(currPosition, glm::vec3(1.5f, 1.0f, 0.25f));

	flipper->SetScale(glm::vec3(2.0f, 1.0f, 1.0f));

	if (usingFlipper) {
		currPosition = glm::scale(currPosition, glm::vec3(1.0f / 1.5f, 1.0f, 1.0f / 0.25f));
		currPosition = glm::translate(currPosition, glm::vec3(-0.9f, 0, 0.9f));
		currPosition = glm::rotate(currPosition, (float)(-90.0f * M_PI / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		currPosition = glm::scale(currPosition, glm::vec3(1.5f, 1.0f, 0.25f));

		flipper->SetScale(glm::vec3(1.0f, 1.0f, 2.0f));
		flipper->SetAngle(1.0f);
	}
	else {
		flipper->SetAngle(0.0f);
	}

	return currPosition;
}

glm::mat4 RTRPhysicsEngine::UseLeftFlipper(bool usingFlipper, RTRCube* flipper) {
	glm::mat4 currPosition = glm::mat4(1.0f);

	currPosition = glm::rotate(currPosition, m_RTRWorld->GetTableAngle(), glm::vec3(1.0f, 0.0f, 0.0f));
	currPosition = glm::translate(currPosition, glm::vec3(-2.55f, -0.75f, 7.85f));
	currPosition = glm::rotate(currPosition, (float)(-15 * M_PI / 180), glm::vec3(0.0f, 1.0f, 0.0f));
	currPosition = glm::scale(currPosition, glm::vec3(1.5f, 1.0f, 0.25f));

	flipper->SetScale(glm::vec3(2.0f, 1.0f, 1.0f));

	if (usingFlipper) {
		currPosition = glm::scale(currPosition, glm::vec3(1.0f / 1.5f, 1.0f, 1.0f / 0.25f));
		currPosition = glm::translate(currPosition, glm::vec3(-0.9f, 0, -0.9f));
		currPosition = glm::rotate(currPosition, (float)(90.0f * M_PI / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		currPosition = glm::scale(currPosition, glm::vec3(1.5f, 1.0f, 0.25f));

		flipper->SetScale(glm::vec3(1.0f, 1.0f, 2.0f));
	}
	else {
		flipper->SetAngle(0.0f);
	}

	return currPosition;
}

void RTRPhysicsEngine::MoveBall(RTRSphere* sphere, float dt)
{
	glm::mat4 transformMatrix = sphere->GetTransformMatrix();
	float zIncrement = 0;
	float yIncrement = 0;
	float xIncrement = 0;

	if (sphere->GetHasCollidedAABB() || sphere->GetHasCollidedSphere()) {
		sphere->SetCanMove(false);
	}
	else {
		sphere->SetCanMove(true);
	}

	if (sphere->GetCanMove()) zIncrement += DownwardsForce(sphere, dt);

	if (sphere->GetMovingForward()) {
		sphere->SetVerticalPower(sphere->GetVerticalPower() + (dt / 1000.0f * GRAVITY * 0.95f));
		zIncrement += dt / 1000.0f * sphere->GetVerticalPower();
		if (fabs(sphere->GetVerticalPower()) <= 0.1f) {
			sphere->SetVerticalPower(0.0f);
			sphere->SetCanMove(false);
			zIncrement = 0;
		}
	}

	if (sphere->GetMovingRight() || sphere->GetMovingLeft()) {
		sphere->SetHorizontalPower(sphere->GetHorizontalPower() - (0.02f * GRAVITY / 10));
	}
	if (sphere->GetMovingRight()) xIncrement += dt / 1000.0f * GRAVITY / 10 * fabs(sphere->GetHorizontalPower());
	else if (sphere->GetMovingLeft()) xIncrement -= dt / 1000.0f * GRAVITY / 10 * fabs(sphere->GetHorizontalPower());
	if (sphere->GetHorizontalPower() <= 0.0f) {
		sphere->SetMovingLeft(false);
		sphere->SetMovingRight(false);
		sphere->SetHorizontalPower(0.0f);
		xIncrement = 0;
	}

	TranslateBall(sphere, xIncrement, yIncrement, zIncrement);
}

void RTRPhysicsEngine::TranslateBall(RTRSphere* sphere, float x, float y, float z) {
	glm::mat4 transformMatrix = sphere->GetTransformMatrix();
	glm::vec3 translation = glm::vec3(x, y, z);
	transformMatrix = glm::translate(transformMatrix, translation);
	sphere->SetTransformMatrix(transformMatrix);
	sphere->SetPosition(glm::vec3(transformMatrix[3]));

	sphere->GetBoundingVolume()->SetTransformMatrix(transformMatrix);
	sphere->GetBoundingVolume()->SetPosition(glm::vec3(transformMatrix[3]));
}

float RTRPhysicsEngine::DownwardsForce(RTRSphere* sphere, float dt) {
	float force = 0;

	force = dt / 1000.0f * GRAVITY / 2.0f * m_RTRWorld->GetTableAngle() * 2.0f;

	return force;
}

void RTRPhysicsEngine::CollisionsAABB(RTRSphere* currBall, RTRObject* object) {
	bool collided = CheckCollisions_AABB_Circle(currBall, object);

	if (collided && !currBall->GetHasCollidedAABB()) {
		currBall->SetHasCollidedAABB(true);
		currBall->SetVerticalPower(currBall->GetVerticalPower() * 0.95f);

		if (object->GetName() == "m_TiltedBlock") {
			currBall->SetVerticalPower(fabs(currBall->GetVerticalPower()));
			currBall->SetMovingForward(true);
			currBall->SetMovingLeft(true);
			currBall->SetDidExit(true);
		}
		if (object->GetName() == "m_TopBar") {
			currBall->SetVerticalPower(fabs(currBall->GetVerticalPower()));
			TranslateBall(currBall, 0, 0, 0.2f);
		}
		else if (object->GetName() == "m_Plunger" || object->GetName() == "m_BottomBar") {
			currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
			if (fabs(currBall->GetVerticalPower()) <= 0.1f) {
				currBall->SetMovingForward(false);
				currBall->SetCanMove(false);
			}
			if (object->GetName() == "m_BottomBar" && currBall->GetDidExit()) {
				if (!currBall->GetMovingForward() &&
					(!currBall->GetMovingLeft() || !currBall->GetMovingRight()) &&
					!currBall->GetCanMove()) {
					currBall->SetDestroyed(true);
				}
			}
		}
		if (object->GetName() == "m_LeftBar") {
			currBall->SetMovingLeft(false);
			currBall->SetMovingRight(true);
			currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.95f);
			TranslateBall(currBall, 0.2f, 0, 0);
		}
		else if (object->GetName() == "m_RightBar" || object->GetName() == "m_SideShootLane") {
			currBall->SetMovingLeft(true);
			currBall->SetMovingRight(false);
			currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.95f);

			if (object->GetName() == "m_RightBar") {
				TranslateBall(currBall, -0.2f, 0, 0);
			}

			if (object->GetName() == "m_SideShootLane") {\
				Collision topCol = CheckTopCollision(currBall, object);
				if (std::get<1>(topCol) == Direction::NORTH) {
					TranslateBall(currBall, -0.2f, 0, -0.2f);
					currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
				}
			}
		}
		if (object->GetName() == "m_RightSlider") {
			currBall->SetVerticalPower(-currBall->GetVerticalPower() * 0.9f);
			currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.9f);
			currBall->SetMovingLeft(!currBall->GetMovingLeft());
			currBall->SetMovingRight(!currBall->GetMovingRight());
			if (fabs(currBall->GetVerticalPower()) <= 0.1f) {
				currBall->SetCanMove(false);
				currBall->SetMovingLeft(true);
				currBall->SetMovingRight(false);
				TranslateBall(currBall, -0.3f, 0, 0.1f);
			}
		}
		if (object->GetName() == "m_LeftSlider") {
			currBall->SetVerticalPower(-currBall->GetVerticalPower() * 0.9f);
			currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.9f);
			currBall->SetMovingLeft(!currBall->GetMovingLeft());
			currBall->SetMovingRight(!currBall->GetMovingRight());
			if (fabs(currBall->GetVerticalPower()) <= 0.1f) {
				currBall->SetCanMove(false);
				currBall->SetMovingLeft(false);
				currBall->SetMovingRight(true);
				TranslateBall(currBall, 0.3f, 0, 0.1f);
			}
		}
		if ((object->GetName() == "m_RightFlipper") || (object->GetName() == "m_LeftFlipper")) {
			if (object->GetAngle() != 0.0f) {
				currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
				currBall->SetVerticalPower(currBall->GetVerticalPower() - 10.0f);
				currBall->SetHorizontalPower(currBall->GetHorizontalPower() + 5.0f);
				currBall->SetMovingForward(true);
				currBall->SetCanMove(true);
				TranslateBall(currBall, 0.0f, 0.0f, -0.1f);
				if (object->GetName() == "m_RightFlipper") {
					currBall->SetMovingLeft(false);
					currBall->SetMovingRight(true);
				}
				else if (object->GetName() == "m_LeftFlipper") {
					currBall->SetMovingLeft(true);
					currBall->SetMovingRight(false);
				}
			}
		}

		if (object->GetName().find("Bumper") != std::string::npos) {
			currBall->SetVerticalPower(currBall->GetVerticalPower() + (currBall->GetVerticalPower() * 0.2f));
			currBall->SetVerticalPower(-currBall->GetVerticalPower());
			currBall->SetHorizontalPower(currBall->GetHorizontalPower() + (currBall->GetHorizontalPower() * 0.2f));
			currBall->SetMovingLeft(!currBall->GetMovingLeft());
			currBall->SetMovingRight(!currBall->GetMovingRight());
		}
	}
}

void RTRPhysicsEngine::CollisionsSpheresSingle(RTRSphere* currBall, RTRSphere* otherBall) {
	if (otherBall->GetName() != currBall->GetName()) {
		Collision collided = CheckCollisions_Circle_Circle(currBall, otherBall);

		if (std::get<0>(collided) && !currBall->GetHasCollidedSphere()) {
			currBall->SetHasCollidedSphere(true);
			otherBall->SetHasCollidedSphere(true);

			if (currBall->GetName().find("Peg") != std::string::npos) {
				PointLightsUp(currBall);
			}
			else {
				PointLightsUp(otherBall);
			}

			Direction dir = std::get<1>(collided);

			if (dir == Direction::NORTH || dir == Direction::NORTH_EAST || dir == Direction::NORTH_WEST) {
				if (currBall->GetName().find("Peg") == std::string::npos) {
					currBall->SetVerticalPower(fabs(currBall->GetVerticalPower()));
				}
				if (otherBall->GetName().find("Peg") == std::string::npos) {
					otherBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
				}
				if (currBall->GetName().find("Peg") == std::string::npos && otherBall->GetName().find("Peg") == std::string::npos) {
					SphereVerticalCol(currBall, otherBall);
				}
				else {
					if (currBall->GetName().find("Peg") == std::string::npos) {
						currBall->SetVerticalPower(currBall->GetVerticalPower() * 0.9f);
					}
					if (otherBall->GetName().find("Peg") == std::string::npos) {
						otherBall->SetVerticalPower(otherBall->GetVerticalPower() * 0.9f);
					}
				}
			}
			if (dir == Direction::SOUTH || dir == Direction::SOUTH_EAST || dir == Direction::SOUTH_WEST) {
				if (currBall->GetName().find("Peg") == std::string::npos) {
					currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
				}
				if (otherBall->GetName().find("Peg") == std::string::npos) {
					otherBall->SetVerticalPower(fabs(currBall->GetVerticalPower()));
				}
				if (currBall->GetName().find("Peg") == std::string::npos && otherBall->GetName().find("Peg") == std::string::npos) {
					SphereVerticalCol(currBall, otherBall);
				}
				else {
					if (currBall->GetName().find("Peg") == std::string::npos) {
						currBall->SetVerticalPower(currBall->GetVerticalPower() * 0.9f);
					}
					if (otherBall->GetName().find("Peg") == std::string::npos) {
						otherBall->SetVerticalPower(otherBall->GetVerticalPower() * 0.9f);
					}
				}
				if (fabs(currBall->GetVerticalPower()) <= 0.1f) {
					currBall->SetMovingForward(false);
					currBall->SetCanMove(false);
				}
			}
			if (dir == Direction::WEST || dir == Direction::NORTH_WEST || dir == Direction::SOUTH_WEST) {
				if (currBall->GetName().find("Peg") == std::string::npos) {
					currBall->SetMovingLeft(false);
					currBall->SetMovingRight(true);
				}
				if (otherBall->GetName().find("Peg") == std::string::npos) {
					otherBall->SetMovingLeft(true);
					otherBall->SetMovingRight(false);
				}
				if (currBall->GetName().find("Peg") == std::string::npos && otherBall->GetName().find("Peg") == std::string::npos) {
					SphereHorizontalCol(currBall, otherBall);
				}
				else {
					if (currBall->GetName().find("Peg") == std::string::npos) {
						currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.9f);
					}
					if (otherBall->GetName().find("Peg") == std::string::npos) {
						otherBall->SetHorizontalPower(otherBall->GetHorizontalPower() * 0.9f);
					}
				}
			}
			if (dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::SOUTH_EAST) {
				if (currBall->GetName().find("Peg") == std::string::npos) {
					currBall->SetMovingLeft(true);
					currBall->SetMovingRight(false);
				}
				if (otherBall->GetName().find("Peg") == std::string::npos) {
					otherBall->SetMovingLeft(false);
					otherBall->SetMovingRight(true);
				}
				if (currBall->GetName().find("Peg") == std::string::npos && otherBall->GetName().find("Peg") == std::string::npos) {
					SphereHorizontalCol(currBall, otherBall);
				}
				else {
					if (currBall->GetName().find("Peg") == std::string::npos) {
						currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.9f);
					}
					if (otherBall->GetName().find("Peg") == std::string::npos) {
						otherBall->SetHorizontalPower(otherBall->GetHorizontalPower() * 0.9f);
					}
				}
			}
		}
	}
}

void RTRPhysicsEngine::SphereVerticalCol(RTRSphere* currBall, RTRSphere* sphere) {
	if (fabs(currBall->GetVerticalPower()) > fabs(sphere->GetVerticalPower())) {
		sphere->SetVerticalPower(sphere->GetVerticalPower() + currBall->GetVerticalPower() * 0.5);
		currBall->SetVerticalPower(currBall->GetVerticalPower() - (currBall->GetVerticalPower() * 0.5));
	}
	else {
		currBall->SetVerticalPower(currBall->GetVerticalPower() + sphere->GetVerticalPower() * 0.5);
		sphere->SetVerticalPower(sphere->GetVerticalPower() - (sphere->GetVerticalPower() * 0.5));
	}
}

void RTRPhysicsEngine::SphereHorizontalCol(RTRSphere* currBall, RTRSphere* sphere) {
	if (fabs(currBall->GetHorizontalPower()) > fabs(sphere->GetHorizontalPower())) {
		sphere->SetHorizontalPower(sphere->GetHorizontalPower() + currBall->GetHorizontalPower() * 0.5);
		currBall->SetHorizontalPower(currBall->GetHorizontalPower() - (currBall->GetHorizontalPower() * 0.5));
	}
	else {
		currBall->SetHorizontalPower(currBall->GetHorizontalPower() + sphere->GetHorizontalPower() * 0.5);
		sphere->SetHorizontalPower(sphere->GetHorizontalPower() - (sphere->GetHorizontalPower() * 0.5));
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

	float totalX = (x - sphere->GetPosition().x) * (x - sphere->GetPosition().x);
	float totalZ = (z - sphere->GetPosition().z) * (z - sphere->GetPosition().z);

	float distance = std::sqrtf(totalX + totalZ);

	return distance < sphere->GetScale().x;
}

Collision RTRPhysicsEngine::CheckTopCollision(RTRSphere* sphere, RTRObject* object) {
	float centerX = sphere->GetPosition().x + sphere->GetScale().x;
	float centerZ = sphere->GetPosition().z + sphere->GetScale().z;
	glm::vec2 center = glm::vec2(centerX, centerZ);

	float object_centerX = object->GetPosition().x + (object->GetScale().x / 2);
	float object_centerZ = object->GetPosition().z + (object->GetScale().z / 2);
	glm::vec2 object_center = glm::vec2(object_centerX, object_centerZ);

	glm::vec2 difference = center - object_center;
	glm::vec2 clamped = glm::clamp(difference, -glm::vec2(object_centerX, object_centerZ), glm::vec2(object_centerX, object_centerZ));
	glm::vec2 closest = object_center + clamped;
	difference = closest - center;

	return std::make_tuple(true, VectorDirection(difference));
}

Collision RTRPhysicsEngine::CheckCollisions_Circle_Circle(RTRSphere* sphere1, RTRSphere* sphere2) {
	glm::vec2 center1(sphere1->GetPosition().x + sphere1->GetScale().x, sphere1->GetPosition().z + sphere1->GetScale().z);
	glm::vec2 center2(sphere2->GetPosition().x + sphere2->GetScale().x, sphere2->GetPosition().z + sphere2->GetScale().z);

	glm::vec2 difference = center1 - center2;

	if (glm::length(difference) < (sphere1->GetScale().x + sphere2->GetScale().x)) {
		return std::make_tuple(true, VectorDirection(difference));
	}
	else {
		return std::make_tuple(false, Direction::NORTH);
	}
}

Direction RTRPhysicsEngine::VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),		// north
		glm::vec2(-1.0f, 1.0f),		// northeast
		glm::vec2(-1.0f, 0.0f),		// east
		glm::vec2(-1.0f, -1.0f),	// southeast
		glm::vec2(0.0f, -1.0f),		// south
		glm::vec2(1.0f, -1.0f),		// southwest
		glm::vec2(1.0f, 0.0f),		// west
		glm::vec2(1.0f, 1.0f)		// northwest
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 8; i++)
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

// 21x15 grid
void RTRPhysicsEngine::SetUpUniformGrid() {
	for (int j = 0; j <= m_GridRow; ++j) {
		std::vector<GridInfo> newGrid;
		for (int i = 0; i <= m_GridColumn; ++i) {
			glm::mat4 model = glm::mat4(1.0f);

			float x = (((float)i / (float)m_GridColumn * (float)m_GridColumn) + -7.25f);
			float y = -1.0f;
			float z = (((float)j / (float)m_GridRow * (float)m_GridRow) + -9.75f);

			model = glm::rotate(model, m_RTRWorld->GetTableAngle(), glm::vec3(1.0f, 0, 0));
			model = glm::translate(model, glm::vec3(x, y, z));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

			glm::vec3 transformedPos = glm::vec3(model[3]);

			newGrid.push_back(std::make_tuple(transformedPos, 0.5f, std::vector<RTRObject*>()));
		}
		m_UniformGrid2D.push_back(newGrid);
	}
}

void RTRPhysicsEngine::PopulateGrid() {
	for (RTRObject* object : m_RTRWorld->GetStaticCollidablePinballObjects()) {
		AddToGrid(object);
	}
}

void RTRPhysicsEngine::AddDynamicsToGrid() {
	for (RTRObject* object : m_RTRWorld->GetDynamicPinballObjects()) {
		AddToGrid(object);
	}

	for (RTRSphere* sphere : m_RTRWorld->GetDynamicObjects()) {
		if (!sphere->GetDestroyed()) {
			AddToGrid(sphere);
		}
	}
}

void RTRPhysicsEngine::UniformGridCollision() {
	for (int j = 0; j <= m_GridRow; ++j) {
		for (int i = 0; i <= m_GridColumn; ++i) {
			std::vector<RTRObject*> objects = std::get<2>(m_UniformGrid2D[j][i]);

			if (objects.size() > 1) {
				for (int i = 0; i < objects.size(); i++) {
					for (int j = i + 1; j < objects.size(); j++) {
						if ((objects.at(i)->GetName().find("newSphere") != std::string::npos) ||
							(objects.at(j)->GetName().find("newSphere") != std::string::npos)) {

							DetermineCollisionType(objects.at(i), objects.at(j));
						}
					}
				}
			}
		}
	}
}

// Disable setHasCollided to test without uniform grid
void RTRPhysicsEngine::DetermineCollisionType(RTRObject* object, RTRObject* object2) {
	if ((object->GetName().find("Sphere") != std::string::npos) &&
		((object2->GetName().find("Sphere") == std::string::npos) && (object2->GetName().find("Peg") == std::string::npos))) {
		RTRSphere* objectSphere = (RTRSphere*)object;
		objectSphere->SetHasCollidedAABB(false);
		objectSphere->SetHasCollidedSphere(false);
		CollisionsAABB((RTRSphere*)objectSphere, object2);
	}
	else if ((object2->GetName().find("Sphere") != std::string::npos) &&
		((object->GetName().find("Sphere") == std::string::npos) && (object->GetName().find("Peg") == std::string::npos))) {
		RTRSphere* objectSphere2 = (RTRSphere*)object2;
		objectSphere2->SetHasCollidedAABB(false);
		objectSphere2->SetHasCollidedSphere(false);
		CollisionsAABB((RTRSphere*)objectSphere2, object);
	}
	else {
		RTRSphere* objectSphere = (RTRSphere*)object;
		RTRSphere* objectSphere2 = (RTRSphere*)object2;
		objectSphere->SetHasCollidedSphere(false);
		objectSphere2->SetHasCollidedSphere(false);
		CollisionsSpheresSingle((RTRSphere*)object, (RTRSphere*)object2);
	}
}

void RTRPhysicsEngine::AddToGrid(RTRObject* currObject)
{
	glm::vec3 objectPosition = currObject->GetPosition();
	float objectScaleX = currObject->GetScale().x;
	float objectScaleZ = currObject->GetScale().z;

	float minObjectX = objectPosition.x - objectScaleX / 2;
	float maxObjectX = objectPosition.x + objectScaleX / 2;
	float minObjectZ = objectPosition.z - objectScaleZ / 2;
	float maxObjectZ = objectPosition.z + objectScaleZ / 2;

	for (int j = 0; j <= m_GridRow; ++j) {
		for (int i = 0; i <= m_GridColumn; ++i) {
			std::vector<RTRObject*> objects = std::get<2>(m_UniformGrid2D[j][i]);
			bool alreadyInserted = false;

			for (RTRObject* object : objects) {
				if (object->GetName() == currObject->GetName()) {
					alreadyInserted = true;
				}
			}

			if (!alreadyInserted) {
				glm::vec3 gridPosition = std::get<0>(m_UniformGrid2D[j][i]);
				float gridScale = std::get<1>(m_UniformGrid2D[j][i]);

				float minGridX = gridPosition.x - (gridScale / 2);
				float maxGridX = gridPosition.x + (gridScale / 2);
				float minGridZ = gridPosition.z - (gridScale / 2);
				float maxGridZ = gridPosition.z + (gridScale / 2);

				if ((currObject->GetName().find("Sphere") != std::string::npos) ||
					(currObject->GetName().find("Peg") != std::string::npos)) {

					float x = std::max(minGridX, std::min(currObject->GetPosition().x, maxGridX));
					float z = std::max(minGridZ, std::min(currObject->GetPosition().z, maxGridZ));

					float totalX = (x - currObject->GetPosition().x) * (x - currObject->GetPosition().x);
					float totalZ = (z - currObject->GetPosition().z) * (z - currObject->GetPosition().z);

					float distance = std::sqrtf(totalX + totalZ);
					
					if (distance < ((RTRSphere*)currObject)->GetScale().x) {
						std::get<2>(m_UniformGrid2D[j][i]).push_back(currObject);
					}
				}
				else {
					if ((minGridX <= maxObjectX && maxGridX > minObjectX) &&
						(minGridZ <= maxObjectZ && maxGridZ >= minObjectZ)) {
						std::get<2>(m_UniformGrid2D[j][i]).push_back(currObject);
					}
				}
			}
		}
	}
}

void RTRPhysicsEngine::ClearDynamicsFromGrid() {
	for (int j = 0; j <= m_GridRow; ++j) {
		for (int i = 0; i <= m_GridColumn; ++i) {
			for (RTRObject* object : std::get<2>(m_UniformGrid2D[j][i])) {
				if (object->GetName().find("Sphere") != std::string::npos || object->GetName().find("Flipper") != std::string::npos) {
					std::get<2>(m_UniformGrid2D[j][i]).erase(std::remove(std::get<2>(m_UniformGrid2D[j][i]).begin(), 
						std::get<2>(m_UniformGrid2D[j][i]).end(), object), std::get<2>(m_UniformGrid2D[j][i]).end());
				}
			}
		}
	}
}

void RTRPhysicsEngine::PointLightsUp(RTRSphere* sphere) {
	int position = 0;

	if (sphere->GetName().find("Zero") != std::string::npos) {
		position = 1;
	}
	else if (sphere->GetName().find("One") != std::string::npos) {
		position = 2;
	}
	else if (sphere->GetName().find("Two") != std::string::npos) {
		position = 3;
	}
	else if (sphere->GetName().find("Three") != std::string::npos) {
		position = 4;
	}
	else if (sphere->GetName().find("Four") != std::string::npos) {
		position = 5;
	}

	m_RTRWorld->GetLightingModel()->GetAllLights()[position].LightsOn = true;
}

void RTRPhysicsEngine::Done() {
	plungerZTrans = 0;
	power = 0;
	m_tableAngle = 0;

	for (int j = 0; j <= m_GridRow; ++j) {
		for (int i = 0; i <= m_GridColumn; ++i) {
			for (RTRObject* objects : std::get<2>(m_UniformGrid2D[j][i])) {
				objects->End();
			}
			std::get<2>(m_UniformGrid2D[j][i]).clear();
		}
		m_UniformGrid2D[j].clear();
	}
	m_UniformGrid2D.clear();

	m_GridRow = 0;
	m_GridColumn = 0;
}