#pragma once

#include "../../glframework/core.h"
#include "camera.h"
#include <map>

class CameraControl {
public:
	CameraControl();
	~CameraControl();

	virtual void onMouse(int button, int action, double xpos , double ypos);
	virtual void onCursor(double xpos, double ypos);
	virtual void onKey(int key, int action, int mods);
	virtual void update();//可以用deltaTime参数控制更新频率？
	virtual void onScroll(float offset);

	void setCamera(Camera* camera) { mCamera = camera; }
	void setSensitivity(float sensitivity) { mSensitivity = sensitivity; }
	void setScaleSpeed(float speed) { mScaleSpeed = speed; }

	//virtual ~CameraControl() {}
	//virtual void onMouseButton(int button, int action, int mods);
	//virtual void onCursorPos(double xpos, double ypos);
	//virtual void onKey(int key, int action, int mods);
	//void setCurrentCamera(Camera* camera) { mCurrentCamera = camera; }
	//Camera* getCurrentCamera() { return mCurrentCamera; }
	//void setSensitivity(float sensitivity) { mSensitivity = sensitivity; }
	//float getSensitivity() { return mSensitivity; }

protected:
	bool mLeftMouseDown = false;
	bool mRightMouseDown = false;
	bool mMiddleMouseDown = false;

	float mCurrentX = 0.0f, mCurrentY = 0.0f;

	float mSensitivity = 0.2f;

	std::map<int, bool> mKeyMap;

	//存储当前控制的那一个摄像机
	Camera* mCamera = nullptr;

	//记录相机缩放速度
	float mScaleSpeed = 0.2f;
};
