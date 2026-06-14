#include "mesh.h"

Mesh::Mesh(Geometry* geometry, Material* material) {
	mGeometry = geometry;
	mMaterial = material;
	mType = ObjectType::Mesh;

	std::cout << "[Ctor] Mesh " << this << std::endl;
}

Mesh::~Mesh() {
	std::cout << "[Dtor] Mesh " << this << std::endl;

	delete mGeometry;
	mGeometry = nullptr;

	delete mMaterial;
	mMaterial = nullptr;
}