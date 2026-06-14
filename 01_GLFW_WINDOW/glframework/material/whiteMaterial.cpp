#include "whiteMaterial.h"

WhiteMaterial::WhiteMaterial() {
	mType = MaterialType::WhiteMaterial;
	std::cout << "[Ctor] WhiteMaterial " << this << std::endl;
}

WhiteMaterial::~WhiteMaterial() {
	std::cout << "[Dtor] WhiteMaterial " << this << std::endl;
}