#include "phongEnvMaterial.h"


PhongEnvMaterial::PhongEnvMaterial() {
	mType = MaterialType::PhongEnvMaterial;
	std::cout << "[Ctor] PhongEnvMaterial " << this << std::endl;
}
PhongEnvMaterial::~PhongEnvMaterial() {
	std::cout << "[Dtor] PhongEnvMaterial " << this << std::endl;
}