#pragma once

#include "camera.h"

class PerspectiveCamera :public Camera {
public:
	PerspectiveCamera(float fovy, float aspect, float near, float far);
	~PerspectiveCamera();

	glm::mat4 getProjectionMatrix() override;

	void scale(float deltaScale) override;

	// === 新增：运行时更新投影参数 ===
	void setPerspective(float fovyDeg, float aspect, float znear, float zfar);
	void setAspect(float aspect);
	void setFovy(float fovyDeg);

private:
	float mFovy = 0.0f;
	float mAspect = 0.0f;
};