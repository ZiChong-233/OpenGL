#include "cameraControl.h"
#include <iostream>



CameraControl::CameraControl() {

}
CameraControl::~CameraControl() {

}

void CameraControl::onMouse(int button, int action, double xpos , double ypos) {
	//判断按键是否按下
	bool pressed = action == GLFW_PRESS ? true : false;

	//记录按下位置
	if (pressed) {
		mCurrentX = xpos;
		mCurrentY = ypos;
	}

	//根据按键去激活不同的记录变量
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		mLeftMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		mRightMouseDown = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		mMiddleMouseDown = pressed;
		break;
	}
}
void CameraControl::onCursor(double xpos, double ypos) {
}
void CameraControl::onKey(int key, int action, int mods) {
	//过滤掉REPEAT
	if(action ==GLFW_REPEAT){
		return;
	}

	//1 检测按下或者释放，给到一个变量
	bool pressed = action == GLFW_PRESS ? true : false;

	//2 记录在keyMap
	mKeyMap[key] = pressed;

}
void CameraControl::update() {
}

void CameraControl::onScroll(float offset) {

}