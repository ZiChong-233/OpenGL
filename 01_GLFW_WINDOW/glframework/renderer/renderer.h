#pragma once
#include <vector>
#include "../core.h"
#include "../mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../light/ambientLight.h"
#include "../shader.h"
#include "../scene.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	//渲染功能函数：
	//每次调用渲染一帧

	void render(
		//const std::vector<Mesh*>& meshes,
		Scene* scene,
		Camera* camera,
		DirectionalLight* dirLight,
		const std::vector<PointLight*>& pointLights,
		SpotLight* spotLight,//目前的学习阶段只实现一个方向光，后续需要考虑会有多种光源传入
		AmbientLight* ambLight,
		unsigned int fbo = 0
	);

	void renderObject(
		Object* object,
		Camera* camera,
		DirectionalLight* dirLight,
		const std::vector<PointLight*>& pointLights,
		SpotLight* spotLight,
		AmbientLight* ambLight
	);

	void setClearColor(const glm::vec3& color);



private:
	void projectObject(Object* obj);//判断物体材质类型，放入对应队列

	//根据材质类型挑选shader
	Shader* pickShader(MaterialType type);

	void setDepthState(Material* material);
	void setPolygonOffsetState(Material* material);
	void setStencilState(Material* material);
	void setBlendState(Material* material);
	void setFaceCullingState(Material* material);

public:
	Material* mGlobalMaterial{ nullptr };//全局材质，可以覆盖所有物体的材质，用于调试

private:
	//生成多种不同的shader对象，根据材质类型不同来挑选使用shader对象
	Shader* mPhongShader{ nullptr };
	Shader* mWhiteShader{ nullptr };
	Shader* mDepthShader{ nullptr };
	Shader* mOpacityMaskShader{ nullptr };
	Shader* mScreenShader{ nullptr };
	Shader* mCubeShader{ nullptr };
	Shader* mPhongEnvShader{ nullptr };



	//不透明物体与透明物体的队列
	//注意，每一帧绘制前，需要清空两个队列
	std::vector<Mesh*> mOpacityObjects{};
	std::vector<Mesh*> mTransparentObjects{};
};


//留档
	//void render(
	//	const std::vector<Mesh*>& meshes,
	//	Camera* camera,
	//	DirectionalLight* dirLight,//目前的学习阶段只实现一个方向光，后续需要考虑会有多种光源传入
	//	AmbientLight* ambLight
	//);
	//void render(
	//	const std::vector<Mesh*>& meshes,
	//	Camera* camera,
	//	PointLight* pointLight,//目前的学习阶段只实现一个方向光，后续需要考虑会有多种光源传入
	//	AmbientLight* ambLight
	//);