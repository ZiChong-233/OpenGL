#include "opacityMaskMaterial.h"


OpacityMaskMaterial::OpacityMaskMaterial() {
	mType = MaterialType::OpacityMaskMaterial;
	std::cout << "[Ctor] OpacityMaterial " << this << std::endl;
}
OpacityMaskMaterial::~OpacityMaskMaterial() {
	std::cout << "[Dtor] OpacityMaterial " << this << std::endl;
}