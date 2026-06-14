#include "depthMaterial.h"

DepthMaterial::DepthMaterial() {
	mType = MaterialType::DepthMaterial;
	std::cout << "[Ctor] DepthMaterial " << this << std::endl;
}

DepthMaterial::~DepthMaterial() {
	std::cout << "[Dtor] DepthMaterial " << this << std::endl;
}