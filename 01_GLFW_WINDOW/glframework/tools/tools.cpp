#include "tools.h"

void Tools::decompose(
	const glm::mat4& matrix,
	glm::vec3& position,
	glm::vec3& eulerAngle,
	glm::vec3& scale) 
{
	glm::quat quaternion;//四元数，用来表示旋转变换
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(matrix, scale, quaternion, position, skew, perspective);

	//四元数->欧拉角
	glm::mat4 rotation = glm::toMat4(quaternion);
	glm::extractEulerAngleXYZ(rotation, eulerAngle.x, eulerAngle.y, eulerAngle.z);

	//解构出来的角度是弧度制，转换为角度制
	eulerAngle = glm::degrees(eulerAngle);
}