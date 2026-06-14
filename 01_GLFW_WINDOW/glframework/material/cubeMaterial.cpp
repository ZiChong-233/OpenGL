#include "cubeMaterial.h"

CubeMaterial::CubeMaterial() {
	mType = MaterialType::CubeMaterial;
	std::cout << "[Ctor] CubeMaterial " << this << std::endl;
}

CubeMaterial::~CubeMaterial() {
	std::cout << "[Dtor] CubeMaterial " << this << std::endl;
}