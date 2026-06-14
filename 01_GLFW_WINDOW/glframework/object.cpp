#include "object.h"


Object::Object() {
	mType = ObjectType::Object;
	std::cout << "[Ctor] Object " << this << std::endl;
}
Object::~Object() {
	for (auto* child : mChildren) {
		delete child;
	}
	mChildren.clear();
	std::cout << "[Dtor] Object " << this << std::endl;
}

void Object::setPosition(const glm::vec3& pos) {//pos是世界坐标系下的坐标
	mPosition = pos;	
}
//增量旋转
void Object::rotateX(float angle) {
	mAngleX += angle;
}
void Object::rotateY(float angle) {
	mAngleY += angle;
}
void Object::rotateZ(float angle) {
	mAngleZ += angle;
}

void Object::setAngleX(float angle) {
	mAngleX = angle;
}
void Object::setAngleY(float angle) {
	mAngleY = angle;
}
void Object::setAngleZ(float angle) {
	mAngleZ = angle;
}

void Object::setScale(glm::vec3 scale) {
	mScale = scale;
}

glm::mat4 Object::getModelMatrix() const {
	//首先获取父节点的变换矩阵
	glm::mat4 parentMatrix{ 1.0f };
	if(mParent != nullptr){
		parentMatrix = mParent->getModelMatrix();
	}

	//unity：缩放-旋转-平移
	glm::mat4 transform{ 1.0f };

	transform = glm::scale(transform, mScale);

	//unity旋转：pitch yaw roll
	transform = glm::rotate(transform, glm::radians(mAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(mAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(mAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));

	transform = parentMatrix * glm::translate(glm::mat4(1.0f), mPosition) * transform;//统一移动到世界坐标系位置  **？
	
	return transform;
}


void Object::addChild(Object* child){
	if (child == nullptr) {
		std::cerr << "Null child added!" << std::endl;
		return;
	}
	//1 检查是否加入过
	auto iter = std::find(mChildren.begin(), mChildren.end(), child);
	if (iter != mChildren.end()) {
		std::cerr << "Duplicate child added!" << std::endl;
		return;
	}

	if (child->mParent != nullptr) {
		std::cerr << "Child already has a parent!" << std::endl;
		return;
	}//**注意：一个节点只能有一个父节点，如果一个节点已经有父节点了，就不能再被加入到另一个节点了*/

	//2 加入
	mChildren.push_back(child);

	//3 设置父节点
	child->mParent = this;

}