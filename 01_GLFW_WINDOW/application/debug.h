#pragma once 
#include "core.h"

struct DebugState {
	glm::vec3 clearColor = glm::vec3(0.2f, 0.1f, 0.1f);

	bool showSkybox = true;
	bool showMainModel = true;

	bool enableDepthTest = true;
	bool enableWireframe = false;
	bool enablePostProcess = false;

	float cameraFov = 60.0f;
	float cameraSpeed = 0.3f;
};//调试状态变量，存储在一个结构体中，方便传递和管理==后续可拆分为多个面板（camera、render、etc.