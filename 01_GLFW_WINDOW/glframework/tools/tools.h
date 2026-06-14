#pragma once
#include "../core.h"

class Tools {
public:
	//传入一个矩阵，解构其中的位置、旋转、缩放信息
	static void decompose(const glm::mat4& matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);

};