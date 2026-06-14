#include "renderer.h"
#include <iostream>
#include "../material/phongMaterial.h"//为什么在cpp中包含？不在h中包含会报错吗？
#include "../material/whiteMaterial.h"
#include "../material/depthMaterial.h"//是否需要？
#include "../material/opacityMaskMaterial.h"
#include "../material/screenMaterial.h"
#include "../material/cubeMaterial.h"
#include "../material/phongEnvMaterial.h"
#include "../../wrapper/checkError.h"
#include <string>
#include <algorithm>


Renderer::Renderer() {
	std::cout << "[Ctor] Renderer " << this << std::endl;

	mPhongShader = new Shader(
		"assets/shaders/phong.vert",
		"assets/shaders/phong.frag"
	);
	mWhiteShader = new Shader(
		"assets/shaders/white.vert",
		"assets/shaders/white.frag"
	);
	mDepthShader = new Shader(
		"assets/shaders/depth.vert",
		"assets/shaders/depth.frag"
	);
	mOpacityMaskShader = new Shader(
		"assets/shaders/phongOpacityMask.vert",
		"assets/shaders/phongOpacityMask.frag"
	);
	mScreenShader = new Shader(
		"assets/shaders/screen.vert",
		"assets/shaders/screen.frag"
	);
	mCubeShader = new Shader(
		"assets/shaders/cube.vert",
		"assets/shaders/cube.frag"
	);
	mPhongEnvShader = new Shader(
		"assets/shaders/phongEnv.vert",
		"assets/shaders/phongEnv.frag"
	);
}

Renderer::~Renderer() {
	std::cout << "[Dtor] Renderer " << this << std::endl;

	delete mPhongShader;
	mPhongShader = nullptr;

	delete mWhiteShader;
	mWhiteShader = nullptr;

	delete mDepthShader;
	mDepthShader = nullptr;

	delete mOpacityMaskShader;
	mOpacityMaskShader = nullptr;

	delete mScreenShader;
	mScreenShader = nullptr;

	delete mCubeShader;
	mCubeShader = nullptr;

	delete mPhongEnvShader;
	mPhongEnvShader = nullptr;
}

void Renderer::setClearColor(const glm::vec3& color) {
	glClearColor(color.r, color.g, color.b, 1.0f);
}


void Renderer::render(
	//const std::vector<Mesh*>& meshes,
	Scene* scene,
	Camera* camera,
	DirectionalLight* dirLight,
	const std::vector<PointLight*>& pointLights,
	SpotLight* spotLight,
	AmbientLight* ambLight,
	unsigned int fbo
) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//1,设置当前帧绘制时OpenGL的必要状态机参数
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_POLYGON_OFFSET_LINE);

	//开启测试、设置基本写入状态，打开模版测试写入
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilMask(0xFF);//保证模版缓冲可以被清理

	//默认颜色混合
	glDisable(GL_BLEND);

	//面剔除实验
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//2，清理画布
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	

	//清空两个队列
	mOpacityObjects.clear();
	mTransparentObjects.clear();

	projectObject(scene);

	sort(mTransparentObjects.begin(), mTransparentObjects.end(),
		[camera](Mesh* a, Mesh* b) {
			auto viewMatrix = camera->getViewMatrix();
			//1 计算A的相机系的Z值
			auto modelMatrixA = a->getModelMatrix();
			auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionA = viewMatrix * worldPositionA;

			//2 计算B的相机系的Z值
			auto modelMatrixB = b->getModelMatrix();
			auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionB = viewMatrix * worldPositionB;

			return cameraPositionA.z < cameraPositionB.z;//升序排列
		}
	);

	//渲染两个队列
	for (int i = 0; i < mOpacityObjects.size(); i++) {
		renderObject(mOpacityObjects[i], camera, dirLight, pointLights, spotLight, ambLight);
	}
	for (int i = 0; i < mTransparentObjects.size(); i++) {
		renderObject(mTransparentObjects[i], camera, dirLight, pointLights, spotLight, ambLight);
	}

}

void Renderer::projectObject(Object* obj) {
	if(obj->getType() == ObjectType::Mesh) {
		Mesh* mesh = (Mesh*)obj;
		auto material = mesh->mMaterial;

		// ✅加在这里：分队列前打印一次
        //std::cout
        //  << " blend=" << material->mBlend
        //  << " depthTest=" << material->mDepthTest
        //  << " depthWrite=" << material->mDepthWrite
        //  << " depthFunc=" << material->mDepthFunc
        //  << " opacity=" << material->mOpacity
        //  << "\n";

		if(material->mBlend) {
			mTransparentObjects.push_back(mesh);
		}
		else {
			
			mOpacityObjects.push_back(mesh);
		}
	}

	auto children = obj->getChildren();	
	for(int i = 0; i < children.size(); i++) {
		projectObject(children[i]);
	}


}

Shader* Renderer::pickShader(MaterialType type) {
	Shader* result = nullptr;

	switch (type)
	{
		case MaterialType::PhongMaterial:
			result = mPhongShader;
			break;

		case MaterialType::WhiteMaterial:
			result = mWhiteShader;
			break;

		case MaterialType::DepthMaterial:
			result = mDepthShader;
			break;
		case MaterialType::OpacityMaskMaterial:
			result = mOpacityMaskShader;
			break;
		case MaterialType::ScreenMaterial:
			result = mScreenShader;
			break;
		case MaterialType::CubeMaterial:
			result = mCubeShader;
			break;
		case MaterialType::PhongEnvMaterial:
			result = mPhongEnvShader;
			break;

		default:
			std::cout<<"Unkown material type to pick shader" << std::endl;
			break;
	}

	return result;
}

//针对单个Object的渲染函数
void Renderer::renderObject(
	Object* object,
	Camera* camera,
	DirectionalLight* dirLight,
	const std::vector<PointLight*>& pointLights,
	SpotLight* spotLight,
	AmbientLight* ambLight
) {
	//判断是否是Mesh对象，如果是则进行渲染
	if (object->getType() == ObjectType::Mesh) {
		//先把东西“拿出来”
		auto mesh = (Mesh*)object;
		auto geometry = mesh->mGeometry;

		Material* material = nullptr;
		if(mGlobalMaterial != nullptr) {
			material = mGlobalMaterial;
		} else {
			material = mesh->mMaterial;
		}

		//auto material = mesh->mMaterial;

		//设置渲染状态
		setDepthState(material);
		setPolygonOffsetState(material);
		setStencilState(material);
		setBlendState(material);	
		setFaceCullingState(material);


		//1 决定使用哪一个shader
		Shader* shader = pickShader(material->mType);

		//2 更新shader的uniform参数
		shader->begin();

		switch (material->mType)
		{
		case MaterialType::PhongMaterial:
		{
			PhongMaterial* phongMat = static_cast<PhongMaterial*>(material);//static_cast的作用？它是必须的吗？
			//diffuse贴图
	//将纹理采样器绑定到对应的纹理单元上
			shader->setInt("sampler", 0);


			//将纹理绑定到对应的单元上
			phongMat->mDiffuse->bind();

			//高光蒙版帧更新
			shader->setInt("specularMaskSampler", 1);
			phongMat->mSpecularMask->bind();

			//shader->setFloat("time", glfwGetTime());
			//shader->setFloat("width", (float)texture->getWidth());
			//shader->setFloat("height", (float)texture->getHeight());

			//MVP
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			//====spotLight的更新
			shader->setVector3("spotLight.position", spotLight->getPosition());
			shader->setVector3("spotLight.color", spotLight->mColor);
			shader->setFloat("spotLight.specularIntensity", spotLight->mSpecularIntensity);

			shader->setVector3("spotLight.targetDirection", spotLight->mTargetDirection);
			shader->setFloat("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
			shader->setFloat("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));

			//====directionalLight的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

			//====pointLight的更新
			for (int i = 0; i < pointLights.size(); i++) {
				std::string indexStr = std::to_string(i);
				auto pointLight = pointLights[i];
				shader->setVector3("pointLights[" + indexStr + "].color", pointLight->mColor);
				shader->setVector3("pointLights[" + indexStr + "].position", pointLight->getPosition());
				shader->setFloat("pointLights[" + indexStr + "].specularIntensity", pointLight->mSpecularIntensity);
				shader->setFloat("pointLights[" + indexStr + "].k2", pointLight->mK2);
				shader->setFloat("pointLights[" + indexStr + "].k1", pointLight->mK1);
				shader->setFloat("pointLights[" + indexStr + "].kc", pointLight->mKc);
			}
			//shader->setVector3("pointLight.color", pointLight->mColor);
			//shader->setVector3("pointLight.position", pointLight->getPosition());
			//shader->setFloat("pointLight.specularIntensity", pointLight->mSpecularIntensity);
			//shader->setFloat("pointLight.k2", pointLight->mK2);
			//shader->setFloat("pointLight.k1", pointLight->mK1);
			//shader->setFloat("pointLight.kc", pointLight->mKc);

			shader->setVector3("ambientColor", ambLight->mColor);//?
			shader->setFloat("shiness", phongMat->mShiness);


			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

			//透明度更新
			shader->setFloat("opacity", material->mOpacity);

		}
		break;
		case MaterialType::WhiteMaterial: {
			//MVP
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());
		}
		break;

		case MaterialType::DepthMaterial: {
			//MVP
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			shader->setFloat("near", camera->mNear);
			shader->setFloat("far", camera->mFar);
		}
		break;

		case MaterialType::OpacityMaskMaterial:
		{
			OpacityMaskMaterial* opacityMaskMat = static_cast<OpacityMaskMaterial*>(material);//static_cast的作用？它是必须的吗？
			//diffuse贴图
	//将纹理采样器绑定到对应的纹理单元上
			shader->setInt("sampler", 0);


			//将纹理绑定到对应的单元上
			opacityMaskMat->mDiffuse->bind();

			//opacityMask帧更新
			shader->setInt("opacityMaskSampler", 1);
			opacityMaskMat->mOpacityMask->bind();

			//MVP
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			//====spotLight的更新
			shader->setVector3("spotLight.position", spotLight->getPosition());
			shader->setVector3("spotLight.color", spotLight->mColor);
			shader->setFloat("spotLight.specularIntensity", spotLight->mSpecularIntensity);

			shader->setVector3("spotLight.targetDirection", spotLight->mTargetDirection);
			shader->setFloat("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
			shader->setFloat("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));

			//====directionalLight的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

			//====pointLight的更新
			for (int i = 0; i < pointLights.size(); i++) {
				std::string indexStr = std::to_string(i);
				auto pointLight = pointLights[i];
				shader->setVector3("pointLights[" + indexStr + "].color", pointLight->mColor);
				shader->setVector3("pointLights[" + indexStr + "].position", pointLight->getPosition());
				shader->setFloat("pointLights[" + indexStr + "].specularIntensity", pointLight->mSpecularIntensity);
				shader->setFloat("pointLights[" + indexStr + "].k2", pointLight->mK2);
				shader->setFloat("pointLights[" + indexStr + "].k1", pointLight->mK1);
				shader->setFloat("pointLights[" + indexStr + "].kc", pointLight->mKc);
			}

			shader->setVector3("ambientColor", ambLight->mColor);//?
			shader->setFloat("shiness", opacityMaskMat->mShiness);


			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

			//透明度更新
			shader->setFloat("opacity", material->mOpacity);

		}
		break;
		case MaterialType::ScreenMaterial:{
			ScreenMaterial* screenMat = static_cast<ScreenMaterial*>(material);
			shader->setInt("screenTexSampler", 0);
			shader->setFloat("texWidth", (float)screenMat->mScreenTexture->getWidth());
			shader->setFloat("texHeight", (float)screenMat->mScreenTexture->getHeight());//这是ai自动给出的流程
			//shader->setFloat("texWidth", 1600);
			//shader->setFloat("texHeight", 1200);
			screenMat->mScreenTexture->bind();
		}
		break;
		case MaterialType::CubeMaterial: {
			CubeMaterial* cubeMat = static_cast<CubeMaterial*>(material);
			mesh->setPosition(camera->mPosition);//让立方体始终跟随相机位置
			//MVP
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			shader->setInt("cubeSampler", 0);
			cubeMat->mDiffuse->bind();
		}
		break;
		case MaterialType::PhongEnvMaterial:
		{
			PhongEnvMaterial* phongMat = static_cast<PhongEnvMaterial*>(material);

			shader->setInt("sampler", 0);


			phongMat->mDiffuse->bind();
			//先暂时关闭高光
			//shader->setInt("specularMaskSampler", 1);
			//phongMat->mSpecularMask->bind();

			shader->setInt("envSampler", 1);
			phongMat->mEnv->bind();

			//MVP
			shader->setMatrix4x4("modelMatrix", mesh->getModelMatrix());
			shader->setMatrix4x4("viewMatrix", camera->getViewMatrix());
			shader->setMatrix4x4("projectionMatrix", camera->getProjectionMatrix());

			auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->getModelMatrix())));
			shader->setMatrix3x3("normalMatrix", normalMatrix);

			//光源参数的uniform更新
			//====spotLight的更新
			shader->setVector3("spotLight.position", spotLight->getPosition());
			shader->setVector3("spotLight.color", spotLight->mColor);
			shader->setFloat("spotLight.specularIntensity", spotLight->mSpecularIntensity);

			shader->setVector3("spotLight.targetDirection", spotLight->mTargetDirection);
			shader->setFloat("spotLight.innerLine", glm::cos(glm::radians(spotLight->mInnerAngle)));
			shader->setFloat("spotLight.outerLine", glm::cos(glm::radians(spotLight->mOuterAngle)));

			//====directionalLight的更新
			shader->setVector3("directionalLight.color", dirLight->mColor);
			shader->setVector3("directionalLight.direction", dirLight->mDirection);
			shader->setFloat("directionalLight.specularIntensity", dirLight->mSpecularIntensity);

			//====pointLight的更新
			for (int i = 0; i < pointLights.size(); i++) {
				std::string indexStr = std::to_string(i);
				auto pointLight = pointLights[i];
				shader->setVector3("pointLights[" + indexStr + "].color", pointLight->mColor);
				shader->setVector3("pointLights[" + indexStr + "].position", pointLight->getPosition());
				shader->setFloat("pointLights[" + indexStr + "].specularIntensity", pointLight->mSpecularIntensity);
				shader->setFloat("pointLights[" + indexStr + "].k2", pointLight->mK2);
				shader->setFloat("pointLights[" + indexStr + "].k1", pointLight->mK1);
				shader->setFloat("pointLights[" + indexStr + "].kc", pointLight->mKc);
			}
			//shader->setVector3("pointLight.color", pointLight->mColor);
			//shader->setVector3("pointLight.position", pointLight->getPosition());
			//shader->setFloat("pointLight.specularIntensity", pointLight->mSpecularIntensity);
			//shader->setFloat("pointLight.k2", pointLight->mK2);
			//shader->setFloat("pointLight.k1", pointLight->mK1);
			//shader->setFloat("pointLight.kc", pointLight->mKc);

			shader->setVector3("ambientColor", ambLight->mColor);//?
			shader->setFloat("shiness", phongMat->mShiness);


			//相机信息更新
			shader->setVector3("cameraPosition", camera->mPosition);

			//透明度更新
			shader->setFloat("opacity", material->mOpacity);

		}
		break;

		default:
			break;
		}

		//3 绑定vao
		glBindVertexArray(geometry->getVao());

		//4 执行绘制命令
		glDrawElements(
			GL_TRIANGLES,
			geometry->getIndicesCount(),
			GL_UNSIGNED_INT,
			0
		);
	}

	////2 遍历当前Object的子节点，递归调用renderObject进行渲染
	//auto children = object->getChildren();
	//for(int i  = 0; i < children.size(); i++) {
	//
	//	renderObject(
	//		children[i],
	//		camera,
	//		dirLight,
	//		pointLights,
	//		spotLight,
	//		ambLight
	//	);
	//}
	//由于所有的mesh均被拆分到两个数列里去了，因此不需要再进行子节点的遍历了

}

void Renderer::setDepthState(Material* material) {
	//1 检测深度状态
	if (material->mDepthTest) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(material->mDepthFunc);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	if (material->mDepthWrite) {
		glDepthMask(GL_TRUE);
	}
	else {
		glDepthMask(GL_FALSE);
	}
}
void Renderer::setPolygonOffsetState(Material* material) {

	//2 检测PolygonOffset状态
	if (material->mPolygonOffsetEnable) {
		glEnable(material->mPolygonOffsetType);
		glPolygonOffset(
			material->mPolygonOffsetFactor,
			material->mPolygonOffsetUnits
		);

	}
	else {
		//glDisable(material->mPolygonOffsetType);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_POLYGON_OFFSET_LINE);//提问，此处为什么要禁用两种类型？只禁用mPolygonOffsetType不行吗？
	}
}

void Renderer::setStencilState(Material* material) {
	if (material->mStencilTestEnable) {
		glEnable(GL_STENCIL_TEST);

		glStencilOp(material->mSFail,material->mZFail,material->mZPass);
		glStencilMask(material->mStencilMask);
		glStencilFunc(material->mStencilFunc, material->mStencilRef, material->mStencilFuncMask);

	}
	else {
		glDisable(GL_STENCIL_TEST);
	}
}

void Renderer::setBlendState(Material* material) {
	if (material->mBlend) {
		glEnable(GL_BLEND);
		glBlendFunc(material->mSFactor,material->mDFactor);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void Renderer::setFaceCullingState(Material* material) {
	if (material->mFaceCullingEnable) {
		glEnable(GL_CULL_FACE);
		glFrontFace(material->mFrontFace);
		glCullFace(material->mCullFace);
	}
	else {
		glDisable(GL_CULL_FACE);
	}

}