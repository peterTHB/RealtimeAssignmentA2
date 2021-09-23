#pragma once
#include "RTRApp.h"
#include "RTRShader.h"
#include "RTRCamera.h"
#include "RTRLighting.h"
#include "RTRObject.h"
#include "Console.h"

class RTRRenderer {
public:
	RTRRenderer();
	void SetUp();
	void RenderWithShaders(RTRShader* shader, glm::mat4 modelMatrix, glm::mat4 viewMatrix,
		glm::mat4 projectionMatrix, RTRObject* object, RTRCamera* camera, RTRLightingModel* lightingModel,
		int curTime, int timeDelta, bool change);
	void DebugInfo(Console* console, int FPS, RTRCamera* camera);

private:
};