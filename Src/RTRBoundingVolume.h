#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include "RTRLighting.h"
#include "RTRShader.h"

struct RTRPoint_t5 {
	float x, y, z, tex1, tex2;
};

struct RTRPoint_t3 {
	float x, y, z;
};

struct RTRFace_t {
	unsigned int v0, v1, v2;
};

class RTRBoundingVolume {
public:
	RTRBoundingVolume(glm::vec3 position, glm::vec3 scale, glm::mat4 matrix) {
		m_Position = position;
		m_Scale = scale;
		m_Matrix = matrix;
	};
	virtual void Init();
	virtual void DrawBoundingVolume(RTRShader* shader);
	virtual void End();
	virtual void SetTransformMatrix(glm::mat4 transformMatrix) { m_Matrix = transformMatrix; };
	virtual glm::mat4 GetTransformMatrix() { return m_Matrix; };
	virtual void SetPosition(glm::vec3 position) { m_Position = position; };
	virtual glm::vec3 GetPosition() { return m_Position; };

protected:
	unsigned int m_VertexBuffer{ 0 };
	unsigned int m_VertexArray{ 0 };
	unsigned int m_FaceElementBuffer{ 0 };
	glm::vec3 m_Position{ 0 };
	glm::vec3 m_Scale{ 0 };
	glm::mat4 m_Matrix{ 0 };
	std::vector<RTRPoint_t3> m_VertexPoints;
	std::vector<RTRFace_t> m_Faces;
	RTRMaterial_t m_Material{ {1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, { 1.0, 1.0, 1.0 }, 64.0 };
};

class RTRBV_AABB : public RTRBoundingVolume {
public:
	RTRBV_AABB(glm::vec3 position, glm::vec3 scale, glm::mat4 matrix) : RTRBoundingVolume(position, scale, matrix) {}
	virtual void Init();
	virtual void DrawBoundingVolume(RTRShader* shader);

private: 

};

class RTRBV_2DCircle : public RTRBoundingVolume {
public:
	RTRBV_2DCircle(glm::vec3 position, glm::vec3 scale, glm::mat4 matrix, int slices, float radius) : RTRBoundingVolume(position, scale, matrix) {
		m_Slices = slices;
		m_Radius = radius;
	}
	virtual void Init();
	virtual void DrawBoundingVolume(RTRShader* shader);
	virtual void End();

private:
	int m_Slices{ 0 };
	float m_Radius{ 0 };
};