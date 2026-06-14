#include "perspectiveCamera.h"


PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far) {
	mFovy = fovy;
	mAspect = aspect;
	mNear = near;
	mFar = far;
}
PerspectiveCamera::~PerspectiveCamera() {

}

glm::mat4 PerspectiveCamera::getProjectionMatrix() {
	return glm::perspective(glm::radians(mFovy), mAspect, mNear, mFar);
}

void PerspectiveCamera::scale(float deltaScale) {
	auto front = glm::cross(mUp, mRight);
	mPosition += front * deltaScale;
}


void PerspectiveCamera::setPerspective(float fovyDeg, float aspect, float znear, float zfar) {
	mFovy = fovyDeg;
	mAspect = aspect;
	mNear = znear;
	mFar = zfar;
}
void PerspectiveCamera::setAspect(float aspect) {
	mAspect = aspect;
}
void PerspectiveCamera::setFovy(float fovyDeg) {
	mFovy = fovyDeg;
}

