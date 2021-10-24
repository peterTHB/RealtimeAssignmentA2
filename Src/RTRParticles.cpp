#include "RTRParticles.h"

void RTRParticles::Update(float dt, RTRSphere* sphere, unsigned int newParticles)
{
	float actualDT = dt / 1000.0f;

	for (unsigned int i = 0; i < newParticles; i++) {
		int unusedParticle = firstUnusedParticle();
		respawnParticle(m_Particles[unusedParticle], sphere);
	}
	for (unsigned int l = 0; l < m_Amount; ++l) {
		Particle& p = m_Particles[l];
		p.Life -= actualDT;
		if (p.Life > 0.0f) {
			p.Position -= glm::vec3(p.Velocity.x, sphere->GetPosition().y, p.Velocity.y) * actualDT;
			p.Color.a -= actualDT * 2.5f;
		}
	}
}

void RTRParticles::Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float scale)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glUseProgram(m_ParShader->GetId());
	for (Particle particle : this->m_Particles) {
		m_ParShader->SetMat4("u_ViewMatrix", viewMatrix);
		m_ParShader->SetMat4("u_Projection", projectionMatrix);
		m_ParShader->SetVec3("u_Offset", particle.Position);
		m_ParShader->SetVec4("u_Color", particle.Color);
		m_ParShader->SetFloat("u_Scale", scale);
		m_ParShader->SetInt("u_Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RTRParticles::Done()
{
	m_Amount = 0;
	m_Texture = 0;
	m_VAO = 0;
	m_LastUsedParticle = 0;

	if (m_VertexPoints != nullptr) { delete m_VertexPoints; m_VertexPoints = nullptr; }
	if (m_Faces != nullptr) { delete m_Faces; m_Faces = nullptr; }
	m_Particles.clear();
}

void RTRParticles::init()
{
	unsigned int VBO;
	unsigned int FEB;

	float m_NumVertices = 6;
	m_VertexPoints = new RTRPoint_t5[]{
		// Position TexCoords
		// Top
		{ -0.5f, 0.0f, 0.5f, 1.0f, 0 }, { 0.5f, 0.0f, 0.5f, 0, 0  }, { 0.5f, 0.0f, -0.5f, 0, 1.0f },
		{ -0.5f, 0.0f, 0.5f, 1.0f, 0 }, { 0.5f, 0.0f, -0.5f, 0, 1.0f }, { -0.5f, 0.0f, -0.5f, 1.0f, 1.0f },
	};

	float m_NumFaces = 2;
	m_Faces = new RTRFace_t[]{
		{ 0, 1, 2 }, {0, 2, 3}
	};

	glGenBuffers(1, &VBO);
	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_NumVertices * sizeof(RTRPoint_t5), m_VertexPoints, GL_STATIC_DRAW);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &FEB);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FEB);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_NumFaces * sizeof(RTRFace_t), m_Faces, GL_STATIC_DRAW);

	for (unsigned int i = 0; i < m_Amount; ++i) {
		m_Particles.push_back(Particle());
	}
}

unsigned int RTRParticles::firstUnusedParticle()
{
	for (unsigned int i = m_LastUsedParticle; i < m_Amount; ++i) {
		if (m_Particles[i].Life <= 0.0f) {
			m_LastUsedParticle = i;
			return i;
		}
	}
	for (unsigned int i = 0; i < m_LastUsedParticle; ++i) {
		if (m_Particles[i].Life <= 0.0f) {
			m_LastUsedParticle = i;
			return i;
		}
	}

	m_LastUsedParticle = 0;
	return 0;
}

void RTRParticles::respawnParticle(Particle& particle, RTRSphere* sphere)
{
	float rColor = 0.5f + ((rand() % 100) / 100.0f);
	particle.Position = sphere->GetPosition();
	particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
	particle.Life = 1.0f;
	particle.Velocity.x = sphere->GetHorizontalPower() * 0.1f;
	particle.Velocity.y = sphere->GetVerticalPower() * 0.1f;
}
