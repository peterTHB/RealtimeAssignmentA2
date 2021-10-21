#include "RTRPhysicsEngine.h"

RTRPhysicsEngine::RTRPhysicsEngine(RTRWorld* rtrworld)
{
	m_RTRWorld = rtrworld;
	plungerZTrans = DEFAULT_PLUNGER_Z_TRANS;
	m_tableAngle = rtrworld->GetAngle();
	SetUpUniformGrid();

	//for (int j = 0; j <= 20; ++j) {
	//	for (int i = 0; i <= 14; ++i) {
	//		glm::vec3 position = std::get<0>(m_UniformGrid2D[j][i]);
	//		std::cout << position.x << "::" << position.y << "::" << position.z << std::endl;
	//	}
	//}
}

glm::mat4 RTRPhysicsEngine::UsePlunger(bool usingPlunger, float timer, glm::mat4 position) {
	glm::vec3 translation = glm::vec3(0, 0, 0);

	if (usingPlunger) {
		float increment = 0;
		if (plungerZTrans < 0.5f) {
			increment = (timer / 1000.0f);
			plungerZTrans += increment;
			power += 1.2f;
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

void RTRPhysicsEngine::MoveBall(RTRSphere* sphere, float dt)
{
	glm::mat4 transformMatrix = sphere->GetTransformMatrix();
	float zIncrement = 0;
	float yIncrement = 0;
	float xIncrement = 0;

	if (sphere->GetCanMove()) zIncrement += DownwardsForce(sphere, dt);

	if (sphere->GetMovingForward()) {
		sphere->SetVerticalPower(sphere->GetVerticalPower() + (dt / 1000.0f * GRAVITY * 0.9f));
		zIncrement += (dt / 1000.0f) * sphere->GetVerticalPower();
		if (fabs(sphere->GetVerticalPower()) <= 0.1f) {
			sphere->SetVerticalPower(0.0f);
			sphere->SetCanMove(false);
			zIncrement = 0;
		}
	}

	if (sphere->GetMovingRight() || sphere->GetMovingLeft()) {
		sphere->SetHorizontalPower(sphere->GetHorizontalPower() - (dt / 1000.0f * GRAVITY / 10));
	}
	if (sphere->GetMovingRight()) xIncrement += (dt / 1000.0f) * GRAVITY / 10 * fabs(sphere->GetHorizontalPower());
	else if (sphere->GetMovingLeft()) xIncrement -= (dt / 1000.0f) * GRAVITY / 10 * fabs(sphere->GetHorizontalPower());
	if (sphere->GetHorizontalPower() <= 0.0f) {
		sphere->SetMovingLeft(false);
		sphere->SetMovingRight(false);
		sphere->SetHorizontalPower(0.0f);
		xIncrement = 0;
	}

	//std::cout << xIncrement << "::" << yIncrement << "::" << zIncrement << std::endl;
	//std::cout << "---------------------" << std::endl;
	//std::cout << sphere->GetName() << std::endl;
	//std::cout << "Can Move: " << sphere->GetCanMove() << std::endl;
	//std::cout << "Has Collided AABB: " << std::get<0>(sphere->GetHasCollided()) << std::endl;
	//std::cout << "Has Collided SPHERE: " << std::get<1>(sphere->GetHasCollided()) << std::endl;
	//std::cout << "Moving Forward: " << sphere->GetMovingForward() << std::endl;
	//std::cout << "Moving Left: " << sphere->GetMovingLeft() << std::endl;
	//std::cout << "Moving Right: " << sphere->GetMovingRight() << std::endl;
	//std::cout << "Vertical Power: " << sphere->GetVerticalPower() << std::endl;
	//std::cout << "Horizontal Power: " << sphere->GetHorizontalPower() << std::endl;
	//std::cout << "Position: " << sphere->GetPosition().x << "::" << sphere->GetPosition().y << "::" << sphere->GetPosition().z << std::endl;
	//std::cout << "---------------------" << std::endl;

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

	force = (dt / 1000.0f) * GRAVITY / 2 * DEFAULT_ANGLE * 1.5f;

	return force;
}

void RTRPhysicsEngine::Collisions(RTRSphere* currBall, std::vector<RTRObject*> objects) {
	for (RTRObject* object : objects) {

		bool collided = CheckCollisions_AABB_Circle(currBall, object);

		if (collided) {
			currBall->SetHasCollidedAABB(true);
			currBall->SetVerticalPower(currBall->GetVerticalPower() * 0.9f);

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
			}
			//else if (object->GetName() == "m_BottomBar") {
			//	currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
			//	if (fabs(currBall->GetVerticalPower()) <= 0.1f) {
			//		currBall->SetMovingForward(false);
			//		currBall->SetCanMove(false);
			//	}
			//}
			if (object->GetName() == "m_LeftBar") {
				currBall->SetMovingLeft(false);
				currBall->SetMovingRight(true);
				currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.9f - GRAVITY / 10);
				TranslateBall(currBall, 0.2f, 0, 0);
			}
			else if (object->GetName() == "m_RightBar" || object->GetName() == "m_SideShootBar") {
				currBall->SetMovingLeft(true);
				currBall->SetMovingRight(false);
				currBall->SetHorizontalPower(currBall->GetHorizontalPower() * 0.9f - GRAVITY / 10);

				if (object->GetName() == "m_RightBar") {
					TranslateBall(currBall, -0.2f, 0, 0);
				}

				if (object->GetName() == "m_SideShootBar") {
					Collision topCol = CheckTopCollision(currBall, object);
					if (std::get<1>(topCol) == Direction::NORTH || 
						std::get<1>(topCol) == Direction::NORTH_EAST || 
						std::get<1>(topCol) == Direction::NORTH_WEST) {
						TranslateBall(currBall, -0.2f, 0, -0.2f);
						currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
					}
				}
			}
		}
	}
}

void RTRPhysicsEngine::CollisionsSpheres(RTRSphere* currBall, std::vector<RTRSphere*> spheres) {
	bool checkCollided = false;

	for (RTRSphere* sphere : spheres) {
		if (sphere->GetName() != currBall->GetName()) {

			Collision collided = CheckCollisions_Circle_Circle(currBall, sphere);

			if (std::get<0>(collided)) {
				checkCollided = true;
				Direction dir = std::get<1>(collided);

				if (dir == Direction::NORTH || dir == Direction::NORTH_EAST || dir == Direction::NORTH_WEST) {
					currBall->SetVerticalPower(fabs(currBall->GetVerticalPower()));
					sphere->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
					SphereVerticalCol(currBall, sphere);
				}
				if (dir == Direction::SOUTH || dir == Direction::SOUTH_EAST || dir == Direction::SOUTH_WEST) {
					currBall->SetVerticalPower(-fabs(currBall->GetVerticalPower()));
					sphere->SetVerticalPower(fabs(currBall->GetVerticalPower()));
					SphereVerticalCol(currBall, sphere);
					if (fabs(currBall->GetVerticalPower()) <= 0.1f) {
						currBall->SetMovingForward(false);
						currBall->SetCanMove(false);
					}
				}
				if (dir == Direction::WEST || dir == Direction::NORTH_WEST || dir == Direction::SOUTH_WEST) {
					currBall->SetMovingLeft(false);
					currBall->SetMovingRight(true);
					sphere->SetMovingLeft(true);
					sphere->SetMovingRight(false);
					SphereHorizontalCol(currBall, sphere);
				}
				if (dir == Direction::EAST || dir == Direction::NORTH_EAST || dir == Direction::SOUTH_EAST) {
					currBall->SetMovingLeft(true);
					currBall->SetMovingRight(false);
					sphere->SetMovingLeft(false);
					sphere->SetMovingRight(true);
					SphereHorizontalCol(currBall, sphere);
				}
			}
		}
	}
	if (!currBall->GetHasCollidedAABB() && !checkCollided) {
		currBall->SetCanMove(true);
	}
}

void RTRPhysicsEngine::SphereVerticalCol(RTRSphere* currBall, RTRSphere* sphere) {
	if (currBall->GetVerticalPower() > sphere->GetVerticalPower()) {
		sphere->SetVerticalPower(sphere->GetVerticalPower() + currBall->GetVerticalPower() * 0.5 - GRAVITY / 10);
		currBall->SetVerticalPower(currBall->GetVerticalPower() - (currBall->GetVerticalPower() * 0.5));
	}
	else {
		currBall->SetVerticalPower(currBall->GetVerticalPower() + sphere->GetVerticalPower() * 0.5 - GRAVITY / 10);
		sphere->SetVerticalPower(sphere->GetVerticalPower() - (sphere->GetVerticalPower() * 0.5));
	}
}

void RTRPhysicsEngine::SphereHorizontalCol(RTRSphere* currBall, RTRSphere* sphere) {
	if (currBall->GetHorizontalPower() > sphere->GetHorizontalPower()) {
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

	return distance < sphere->GetRadius();
}

Collision RTRPhysicsEngine::CheckTopCollision(RTRSphere* sphere, RTRObject* object) {
	float centerX = sphere->GetPosition().x + sphere->GetRadius();
	float centerZ = sphere->GetPosition().z + sphere->GetRadius();
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
	glm::vec2 center1(sphere1->GetPosition().x + sphere1->GetRadius(), sphere1->GetPosition().z + sphere1->GetRadius());
	glm::vec2 center2(sphere2->GetPosition().x + sphere2->GetRadius(), sphere2->GetPosition().z + sphere2->GetRadius());

	glm::vec2 difference = center1 - center2;

	if (glm::length(difference) < (sphere1->GetRadius() + sphere2->GetRadius())) {
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

void RTRPhysicsEngine::SetUpUniformGrid() {
	int row = 20;
	int column = 14;

	for (int j = 0; j <= row; ++j) {
		std::vector<GridInfo> newGrid;
		for (int i = 0; i <= column; ++i) {
			glm::mat4 model = glm::mat4(1);

			float x = ((float)i / (float)column * (float)column) + -6.75f;
			float y = -1.0f;
			float z = ((float)j / (float)row * (float)row) + -9.75f;

			model = glm::rotate(model, DEFAULT_ANGLE, glm::vec3(1.0f, 0, 0));
			model = glm::translate(model, glm::vec3(x, y, z));

			glm::vec3 transformedPos = glm::vec3(model[3]);

			/*m_UniformGrid2D[j][i] = std::make_tuple(transformedPos, 0.5f, std::vector<RTRObject*>());*/
			newGrid.push_back(std::make_tuple(transformedPos, 0.5f, std::vector<RTRObject*>()));
		}
		m_UniformGrid2D.push_back(newGrid);
	}
}

void RTRPhysicsEngine::UniformGridCollision() {

}

void RTRPhysicsEngine::AddToGrid(RTRObject* currObject)
{
	int row = 20;
	int column = 14;

	for (int j = 0; j <= 20; ++j) {
		for (int i = 0; i <= 14; ++i) {
			std::vector<RTRObject*> objects = std::get<2>(m_UniformGrid2D[j][i]);
			bool notIn = false;

			for (RTRObject* object : objects) {
				if (object->GetName() == object->GetName()) {
					notIn = true;
				}
			}

			if (notIn) {
				glm::vec3 position = std::get<0>(m_UniformGrid2D[j][i]);
				float scale = std::get<1>(m_UniformGrid2D[j][i]);

				float centerX = position.x;
				float centerZ = position.z;

				float minX = position.x - scale;
				float maxX = position.x + scale;
				float minZ = position.z - scale;
				float maxZ = position.z + scale;
			}
		}
	}
}

void RTRPhysicsEngine::RemoveFromGrid(RTRObject* currObject)
{
	int row = 20;
	int column = 14;

	int placement = -1;

	for (int j = 0; j <= 20; ++j) {
		for (int i = 0; i <= 14; ++i) {
			glm::vec3 position = std::get<0>(m_UniformGrid2D[j][i]);
			float centerX = position.x;
			float centerZ = position.z;
		}
	}
}

void RTRPhysicsEngine::Done() {
	plungerZTrans = 0;
	power = 0;
	m_tableAngle = 0;

	for (std::vector<GridInfo> grid : m_UniformGrid2D) {
		for (GridInfo info : grid) {
			for (RTRObject* objects : std::get<2>(info)) {
				objects->End();
			}
			std::get<2>(info).clear();
		}
		grid.clear();
	}
	m_UniformGrid2D.clear();
}