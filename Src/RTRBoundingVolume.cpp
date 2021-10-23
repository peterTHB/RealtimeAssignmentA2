#include "RTRBoundingVolume.h"

void RTRBoundingVolume::Init()
{
	glGenBuffers(1, &m_VertexBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_VertexPoints.size() * sizeof(RTRPoint_t3), m_VertexPoints.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_FaceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RTRFace_t), m_Faces.data(), GL_STATIC_DRAW);
}

void RTRBoundingVolume::DrawBoundingVolume(RTRShader* shader)
{
}

void RTRBoundingVolume::End()
{
}

void RTRBV_AABB::Init()
{
	float xMin, xMax, zMin, zMax = 0;

	//xMin = -(m_Scale.x / 2);
	//xMax = (m_Scale.x / 2);
	//zMin = -(m_Scale.z / 2);
	//zMax = (m_Scale.z / 2);

	//RTRPoint_t3 coord1 = { xMin, 0, zMax };
	//RTRPoint_t3 coord2 = { xMax, 0, zMax };
	//RTRPoint_t3 coord3 = { xMax, 0, zMin };
	//RTRPoint_t3 coord4 = { xMin, 0, zMin };

	//xMin = m_Position.x - (m_Scale.x / 2);
	//xMax = m_Position.x + (m_Scale.x / 2);
	//zMin = m_Position.z - (m_Scale.z / 2);
	//zMax = m_Position.z + (m_Scale.z / 2);

	RTRPoint_t3 coord1 = { -0.5f, 0, 0.5f };
	RTRPoint_t3 coord2 = { 0.5f, 0, 0.5f };
	RTRPoint_t3 coord3 = { 0.5f, 0, -0.5f };
	RTRPoint_t3 coord4 = { -0.5f, 0, -0.5f };

	m_VertexPoints.push_back(coord1);
	m_VertexPoints.push_back(coord2);
	m_VertexPoints.push_back(coord3);
	m_VertexPoints.push_back(coord1);
	m_VertexPoints.push_back(coord3);
	m_VertexPoints.push_back(coord4);

	m_Faces.push_back({ 0, 1, 2 });
	m_Faces.push_back({ 0, 2, 3 });

	//glm::mat4 newMatrix = glm::mat4(1.0f);
	//glm::vec3 newPosition = glm::vec3(m_Matrix[3]);
	//newMatrix[3][0] = newPosition.x;
	//newMatrix[3][1] = newPosition.y;
	//newMatrix[3][2] = newPosition.z;

	//m_Matrix = newMatrix;
	//m_Position = newPosition;

	RTRBoundingVolume::Init();
}

void RTRBV_AABB::DrawBoundingVolume(RTRShader* shader)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(shader->GetId());
	glBindVertexArray(m_VertexArray);
	glDrawArrays(GL_TRIANGLES, 0, m_VertexPoints.size());
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RTRBV_2DCircle::Init()
{
	for (int i = 0; i < m_Slices * 1.5; i++) {
		float theta = 2.0f * M_PI * float(i) / float(m_Slices);
		float x = m_Radius * 2 * cosf(theta);
		float z = m_Radius * 2 * sinf(theta);

		RTRPoint_t3 vertex = RTRPoint_t3{ x, 0, z };
		m_VertexPoints.push_back(vertex);
	}

	m_Faces.push_back({ 0, 1, 2 });

	glGenBuffers(1, &m_VertexBuffer);
	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_VertexPoints.size() * sizeof(RTRPoint_t3), m_VertexPoints.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_VertexArray);
	glBindVertexArray(m_VertexArray);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_FaceElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_FaceElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RTRFace_t), m_Faces.data(), GL_STATIC_DRAW);
}

void RTRBV_2DCircle::DrawBoundingVolume(RTRShader* shader)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glUseProgram(shader->GetId());
	glBindVertexArray(m_VertexArray);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_VertexPoints.size());
	glBindVertexArray(0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RTRBV_2DCircle::End()
{
}
