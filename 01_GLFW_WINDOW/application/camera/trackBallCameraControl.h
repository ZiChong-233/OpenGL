#pragma once

#include "cameraControl.h"

class TrackBallCameraControl : public CameraControl {
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();

	void onCursor(double xpos, double ypos) override;

	void onScroll(float offset) override;

	//For IMGUI control
	void onKey(int key, int action, int mods) override;

private:
	void pitch(float angle);
	void yaw(float angle);

private:
	float mMoveSpeed = 0.005f;


	bool mCtrlDown = false;


};