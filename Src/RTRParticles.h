#pragma once
#include <string>
#include <iostream>
#include <glad/glad.h>
#include <sdl/SDL.h>
#include <stb/stb_image.h>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "RTRShader.h"
#include "RTRObject.h"

struct Particle {
	glm::vec3 Position;
	glm::vec2 Velocity;
	glm::vec4 Color;
	float Life;

	Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

class RTRParticles {
public:
	RTRParticles(RTRShader* shader, unsigned int texture, unsigned int amount) { 
		m_ParShader = shader;
		m_Texture = texture;
		m_Amount = amount;
		init();
	};
	~RTRParticles() {};
	virtual void Update(float dt, RTRSphere* sphere, unsigned int newParticles);
	virtual void Render(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, float scale);
	virtual void Done();

private:
	std::vector<Particle> m_Particles;
	RTRShader* m_ParShader{ nullptr };
	RTRPoint_t5* m_VertexPoints{ nullptr };
	RTRFace_t* m_Faces{ nullptr };
	unsigned int m_Amount{ 0 };
	unsigned int m_Texture{ 0 };
	unsigned int m_VAO{ 0 };
	unsigned int m_LastUsedParticle{ 0 };
	void init();
	unsigned int firstUnusedParticle();
	void respawnParticle(Particle& particle, RTRSphere* sphere);
};