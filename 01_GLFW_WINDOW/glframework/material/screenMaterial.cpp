#include "screenMaterial.h"

ScreenMaterial::ScreenMaterial() {
	mType = MaterialType::ScreenMaterial;
	std::cout << "[Ctor] ScreenMaterial " << this << std::endl;
}

ScreenMaterial::~ScreenMaterial() {
	std::cout << "[Dtor] ScreenMaterial " << this << std::endl;
}