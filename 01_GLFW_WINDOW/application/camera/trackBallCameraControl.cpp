#include "trackBallCameraControl.h"

TrackBallCameraControl::TrackBallCameraControl() {

}
TrackBallCameraControl::~TrackBallCameraControl() {

}

void TrackBallCameraControl::onCursor(double xpos, double ypos) {
	if (mLeftMouseDown && mCtrlDown) {
		//1 计算经纬线增量角度
		float deltaX = (xpos - mCurrentX) * mSensitivity;
		float deltaY = (ypos - mCurrentY) * mSensitivity;
		//2 分开pitch和yaw，各自计算
		pitch(-deltaY);
		yaw(-deltaX);//发现旋转方向不合预期，负号取反
	}else if(mMiddleMouseDown && mCtrlDown){
		//平移
		float deltaX = (xpos - mCurrentX) * mMoveSpeed;
		float deltaY = (ypos - mCurrentY) * mMoveSpeed;
		//相机位置和目标位置都要平移
		mCamera->mPosition -= mCamera->mRight * deltaX;
		mCamera->mPosition += mCamera->mUp * deltaY;
	}
	mCurrentX = xpos;
	mCurrentY = ypos;
}

void TrackBallCameraControl::pitch(float angle) {
	//绕着mRight向量在旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), mCamera->mRight);
	//影响当前相机的up向量和位置 
	mCamera->mUp = glm::vec3(mat * glm::vec4(mCamera->mUp, 0.0f));//vec4给vec3只给xyz
	mCamera->mPosition = glm::vec3(mat * glm::vec4(mCamera->mPosition, 1.0f));//表示位置的向量w=1***
}
void TrackBallCameraControl::yaw(float angle) {
	//绕着世界坐标系的Y轴在旋转
	auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	//影响当前相机的up向量和位置 
	mCamera->mUp = glm::vec3(mat * glm::vec4(mCamera->mUp, 0.0f));//vec4给vec3只给xyz
	mCamera->mRight = glm::vec3(mat * glm::vec4(mCamera->mRight, 0.0f));
	mCamera->mPosition = glm::vec3(mat * glm::vec4(mCamera->mPosition, 1.0f));//表示位置的向量w=1***
}

void TrackBallCameraControl::onScroll(float offset) {
	mCamera->scale(mScaleSpeed * offset);
}



//为便于GUI操作，新增ctrl控制
void TrackBallCameraControl::onKey(int key, int action, int mods) {
	if (key == GLFW_KEY_LEFT_CONTROL || key == GLFW_KEY_RIGHT_CONTROL) {
		if (action == GLFW_PRESS)
			mCtrlDown = true;
		else if (action == GLFW_RELEASE)
			mCtrlDown = false;
	}
}
