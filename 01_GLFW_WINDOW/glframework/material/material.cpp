#include "material.h"
Material::Material() {
	std::cout << "[Ctor] Material " << this << std::endl;
}
Material::~Material() {
	std::cout << "[Dtor] Material " << this << std::endl;
}