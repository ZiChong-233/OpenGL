#pragma once
#include "object.h"
#include "geometry.h"
#include "material/material.h"

class Mesh : public Object {
public:		//此处统一用public是为了便于学习，在实际项目中请注意封装性
	Mesh(Geometry* geometry, Material* material);
	~Mesh() override;
public:
	Geometry* mGeometry{ nullptr };
	Material* mMaterial{ nullptr };
};