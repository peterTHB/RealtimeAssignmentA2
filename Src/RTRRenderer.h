#pragma once
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRLighting.h"
#include "RTRObject.h"
#include "RTRBoundingVolume.h"
#include "Console.h"

class RTRRenderer {
public:
	RTRRenderer();
	virtual void SetUp();
	virtual void RenderWithShaders(int shaderPos, glm::mat4 modelMatrix, glm::mat4 viewMatrix,
		glm::mat4 projectionMatrix, RTRObject* object, RTRCamera* camera, RTRLightingModel* lightingModel,
		int curTime, int timeDelta);
	virtual void RenderBoundingBoxes(int shaderPos, glm::mat4 modelMatrix, glm::mat4 viewMatrix,
		glm::mat4 projectionMatrix, RTRBoundingVolume* boundingVolume, RTRCamera* camera, RTRLightingModel* lightingModel,
		int curTime, int timeDelta);
	virtual void RenderSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	virtual void DebugInfo(Console* console, int FPS, RTRCamera* camera);
	virtual void Done();

	std::vector<RTRShader*> ShaderVector;
	RTRShader* m_PinballStaticShader{ nullptr };
	RTRShader* m_PinballDynamicShader{ nullptr };
	RTRShader* m_DynamicObjectsShader{ nullptr };
	RTRShader* m_SkyboxShader{ nullptr };
	RTRShader* m_NoTextureShader{ nullptr };
	RTRShader* m_CircleShader{ nullptr };

private:
	float lastTime{ 0 };
	float timer{ 0 };
};