#include<iostream>

#include"glframework/core.h"
#include"glframework/shader.h"

#include<string>
#include <cassert>//教程使用asser.h，但是cassert更符合c++标准
#include "wrapper/checkError.h"
#include"application/Application.h"
#include"glframework/texture.h"

//引入相机+控制器

#include"application/camera/perspectiveCamera.h"
#include"application/camera/orthographicCamera.h"
#include"application/camera/trackBallCameraControl.h"
#include"application/camera/GameCameraControl.h"

#include "glframework/geometry.h"
#include "glframework/material/phongMaterial.h"
#include "glframework/material/whiteMaterial.h"
#include "glframework/material/depthMaterial.h"
#include "glframework/material/opacityMaskMaterial.h"
#include "glframework/material/screenMaterial.h"
#include "glframework/material/cubeMaterial.h"
#include "glframework/material/phongEnvMaterial.h"

#include "glframework/mesh.h"
#include "glframework/renderer/renderer.h"
#include "glframework/light/pointLight.h"
#include "glframework/light/spotLight.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glframework/scene.h"
#include "application/assimpLoader.h"

#include "glframework/framebuffer/framebuffer.h"



struct AppRuntime {
	Renderer* renderer = nullptr;

	Scene* scene = nullptr;
	Scene* sceneOffscreen = nullptr;
	Scene* sceneInscreen = nullptr;

	Framebuffer* framebuffer = nullptr;

	PerspectiveCamera* camera = nullptr;
	TrackBallCameraControl* cameraControl = nullptr;

	//Mesh* meshWhite = nullptr;
	std::vector<PointLight*> pointLights = {};
	SpotLight* spotLight = nullptr;
	AmbientLight* ambLight = nullptr;
	DirectionalLight* dirLight = nullptr;

};//全局变量，存储运行时状态

struct DebugState {
	glm::vec3 clearColor = glm::vec3(0.2f, 0.1f, 0.1f);

	bool showSkybox = true;
	bool showMainModel = true;

	bool enableDepthTest = true;
	bool enableWireframe = false;
	bool enablePostProcess = false;

	float cameraFov = 60.0f;
	float cameraSpeed = 0.3f;
};//调试状态变量，存储在一个结构体中，方便传递和管理==后续可拆分为多个面板（camera、render、etc.

AppRuntime gRuntime;
DebugState gDebug;

constexpr int INITIAL_WIDTH = 1600;
constexpr int INITIAL_HEIGHT = 1200;


constexpr float BASE_HEIGHT = 600.0f;
constexpr float INITIAL_FOV_DEG = 60.0f;
const float BASE_FOV_RAD = glm::radians(INITIAL_FOV_DEG);
void OnResize(int width, int height) {
	//GL_CALL(glViewport(0, 0, width, height));
	//float aspect = (float)width / (float)height;

	//float fovyNew = 2.0f * atan((height / baseHeight) * tan(baseFovy / 2.0f));
	//perspectiveMatrix = glm::perspective(fovyNew, aspect, 0.1f, 1000.0f);

	glViewport(0, 0, width, height);
	if (height <= 0) return;                     // 防止除零

	if (!gRuntime.camera) return;

	// 2) 计算新的宽高比
	const float aspect = static_cast<float>(width) / static_cast<float>(height);

	// 3) 关键：为了保持屏幕像素尺度不变，随窗口高度线性调整FOV
	float fovyNewRad = 2.0f * atan((height / BASE_HEIGHT) * tan(BASE_FOV_RAD * 0.5f));

	// 4) 可选：夹紧，避免极端尺寸时数值/视觉问题
	fovyNewRad = glm::clamp(fovyNewRad, glm::radians(10.0f), glm::radians(170.0f));

	// 5) 把新投影参数喂给相机
	// ——如果你的 setPerspective 接受“度”：
	gRuntime.camera->setPerspective(glm::degrees(fovyNewRad), aspect, 0.1f, 1000.0f);
	// ——如果你的 setPerspective 接受“弧度”，改成：
	// camera->setPerspective(fovyNewRad, aspect, camera->getNear(), camera->getFar());

	// 6) （可选）如果 TrackBallControl 需要新的视口做弧球映射，也同步一下：
	// cameraControl->setViewport(width, height);
}

void OnKey(int key, int action, int mods) {
	if (!gRuntime.cameraControl) return;
	gRuntime.cameraControl->onKey(key, action, mods);
}
//鼠标按下和鼠标移动
void OnMouse(int button, int action, int mods) {
	if (!gRuntime.cameraControl) return;
	double x, y;
	appGI->getCursorPosition(&x, &y);
	gRuntime.cameraControl->onMouse(button, action, x, y);
}
void OnCursor(double xpos, double ypos) {
	if (!gRuntime.cameraControl) return;
	gRuntime.cameraControl->onCursor(xpos, ypos);
}
//鼠标滚轮
void OnScroll(double offset) {
	if (!gRuntime.cameraControl) return;
	gRuntime.cameraControl->onScroll(offset);
}


void setModelBlend(Object* obj, bool blend, float opacity) {
	if (obj->getType() == ObjectType::Mesh) {
		Mesh* mesh = (Mesh*)obj;
		Material* mat = mesh->mMaterial;
		mat->mBlend = blend;
		mat->mOpacity = opacity;
		mat->mDepthWrite = false;
	}

	auto children = obj->getChildren();
	for (int i = 0; i < children.size(); i++) {
		setModelBlend(children[i], blend, opacity);
	}
}

void setModelWhiteDebug(Object* obj) {
	if (obj->getType() == ObjectType::Mesh) {
		Mesh* mesh = (Mesh*)obj;
		delete mesh->mMaterial;

		auto* whiteMat = new WhiteMaterial();
		whiteMat->mFaceCullingEnable = false;
		mesh->mMaterial = whiteMat;
	}

	auto children = obj->getChildren();
	for (int i = 0; i < children.size(); i++) {
		setModelWhiteDebug(children[i]);
	}
}

void createRenderer(AppRuntime& rt) {
	rt.renderer = new Renderer();
}

void createScene(AppRuntime& rt) {
	rt.scene = new Scene();
}

void buildSceneObjects(AppRuntime& rt) {
	std::vector<std::string> paths = {
	"assets/textures/skybox2/right.jpg",
	"assets/textures/skybox2/left.jpg",
	"assets/textures/skybox2/top.jpg",
	"assets/textures/skybox2/bottom.jpg",
	"assets/textures/skybox2/back.jpg",
	"assets/textures/skybox2/front.jpg"
	};

	//auto sphereGeo = Geometry::createSphere(4.0f);
	//auto sphereMat = new PhongMaterial();
	//sphereMat->mDiffuse = new Texture("assets/textures/earth.png", 0);
	//auto sphereMesh = new Mesh(sphereGeo, sphereMat);
	//scene->addChild(sphereMesh);

	//Texture* envTex1 = new Texture(paths, 0);
	//auto boxGeo = Geometry::createBox(1.0f);
	//auto boxMat = new CubeMaterial();
	//boxMat->mDiffuse = envTex1;
	//boxMat->mDepthWrite = false;
	//auto boxMesh = new Mesh(boxGeo, boxMat);
	//rt.scene->addChild(boxMesh);

	//Texture* envTex2 = new Texture(paths, 1);
	//auto sphereGeo = Geometry::createSphere(4.0f);
	//auto sphereMat = new PhongEnvMaterial();
	//sphereMat->mDiffuse = new Texture("assets/textures/earth.png", 0);
	//sphereMat->mEnv = envTex2;
	//auto sphereMesh = new Mesh(sphereGeo, sphereMat);
	//rt.scene->addChild(sphereMesh);

	auto* model = AssimpLoader::load("assets/fbx/bag/3D_PCB2_2026-03-31.obj");
	if (model) {
		model->setPosition(glm::vec3(-50.7f, 28.5f, 0.8f));
		model->setScale(glm::vec3(0.05f));
		model->setAngleY(0.0f);
		setModelWhiteDebug(model);
		rt.scene->addChild(model);
	}

}

//=================================================================
// light
//=================================================================
void clearLights(AppRuntime& rt) {
	if (rt.dirLight) {
		delete rt.dirLight;
		rt.dirLight = nullptr;
	}

	if (rt.ambLight) {
		delete rt.ambLight;
		rt.ambLight = nullptr;
	}

	if (rt.spotLight) {
		delete rt.spotLight;
		rt.spotLight = nullptr;
	}

	for (auto* light : rt.pointLights) {
		delete light;
	}
	rt.pointLights.clear();
}
PointLight* createPointLight(
	const glm::vec3& position,
	const glm::vec3& color,
	float specularIntensity = 0.2f,
	float kc = 1.0f,
	float k1 = 0.0f,
	float k2 = 0.0f
) {
	auto* light = new PointLight();
	light->setPosition(position);
	light->mColor = color;
	light->mKc = kc;
	light->mK1 = k1;
	light->mK2 = k2;
	light->mSpecularIntensity = specularIntensity;
	return light;
}
void buildLights(AppRuntime& rt) {
	// 先清理旧光源，避免重复构建时内存泄漏或 pointLights 重复追加
	clearLights(rt);

	// Directional Light
	rt.dirLight = new DirectionalLight();
	rt.dirLight->mDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	rt.dirLight->mSpecularIntensity = 0.2f;
	// rt.dirLight->mColor = glm::vec3(1.0f);   // 若需要可显式写出

	// Ambient Light
	rt.ambLight = new AmbientLight();
	rt.ambLight->mColor = glm::vec3(0.7f);

	// Spot Light（当前默认关闭，用黑色表示）
	rt.spotLight = new SpotLight();
	rt.spotLight->setPosition(glm::vec3(4.0f, 0.0f, 0.0f));
	rt.spotLight->mTargetDirection = glm::vec3(0.0f, 0.0f, 1.0f);
	rt.spotLight->mInnerAngle = 20.0f;
	rt.spotLight->mOuterAngle = 25.0f;
	rt.spotLight->mColor = glm::vec3(0.0f);

	// Point Lights（当前默认也关闭）
	rt.pointLights.push_back(
		createPointLight(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f))
	);

	rt.pointLights.push_back(
		createPointLight(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f))
	);

	rt.pointLights.push_back(
		createPointLight(glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f))
	);

	rt.pointLights.push_back(
		createPointLight(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f))
	);
}

void buildCamera(AppRuntime& rt) {
	rt.camera = new PerspectiveCamera(
		INITIAL_FOV_DEG,
		(float)appGI->getWidth() / (float)appGI->getHeight(),
		0.1f,
		1000.0f
	);
	float size = 6.0f;
	//camera = new OrthographicCamera(
	//	-size, size,
	//	size,-size,
	//	size, -size
	//);
	rt.cameraControl = new TrackBallCameraControl();
	rt.cameraControl->setCamera(rt.camera);
	rt.cameraControl->setSensitivity(0.5f);
}

//==================================================================
// ImGui
//==================================================================
void initIMGUI() {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(appGI->getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");//含义：使用4.6版本的GLSL（OpenGL
}

void renderIMGUI(DebugState& debug) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Render");
	ImGui::ColorEdit3("Clear Color", (float*)&debug.clearColor);
	ImGui::End();

	ImGui::Render();

	int display_w, display_h;
	glfwGetFramebufferSize(appGI->getWindow(), &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void shutdown(AppRuntime& rt) {
	//delete 一个 nullptr 是安全的，因此这里就不做判空处理了
	//  // 先关闭 ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// 先清理点光源容器
	for (auto* light : rt.pointLights) {
		delete light;
	}
	rt.pointLights.clear();

	// 清理其他光源
	delete rt.spotLight;
	rt.spotLight = nullptr;

	delete rt.ambLight;
	rt.ambLight = nullptr;

	delete rt.dirLight;
	rt.dirLight = nullptr;

	// 清理相机与控制器
	delete rt.cameraControl;
	rt.cameraControl = nullptr;

	delete rt.camera;
	rt.camera = nullptr;

	// 清理 framebuffer
	delete rt.framebuffer;
	rt.framebuffer = nullptr;

	// 清理场景
	delete rt.sceneOffscreen;
	rt.sceneOffscreen = nullptr;

	delete rt.sceneInscreen;
	rt.sceneInscreen = nullptr;

	delete rt.scene;
	rt.scene = nullptr;

	// 最后清理渲染器
	delete rt.renderer;
	rt.renderer = nullptr;

	Texture::clearCache();
}

int main() {


	if (!appGI->init(INITIAL_WIDTH, INITIAL_HEIGHT)) {
		return -1;
	}

	appGI->setResizeCallback(OnResize);
	appGI->setKeyBoardCallback(OnKey);
	appGI->setMouseCallback(OnMouse);
	appGI->setCursorCallback(OnCursor);
	appGI->setScrollCallback(OnScroll);

	//设置OpenGL视口以及清理颜色
	GL_CALL(glViewport(0, 0, INITIAL_WIDTH, INITIAL_HEIGHT));
	GL_CALL(glClearColor(0.2f, 0.1f, 0.1f, 1.0f));//RGBA，A是Alpha，透明度

	createRenderer(gRuntime);
	createScene(gRuntime);

	buildCamera(gRuntime);
	buildLights(gRuntime);
	buildSceneObjects(gRuntime);

	initIMGUI();


	while (appGI->update()) {
		//meshes[1]->rotateZ(0.2f);
		//meshes[0]->rotateY(0.6f);
		gRuntime.cameraControl->update();
		gRuntime.renderer->setClearColor(gDebug.clearColor);
		//pass01 将box渲染到colorAttachment纹理上，也就是FBO上
		//renderer->render(sceneOffscreen, camera, dirLight, pointLights, spotLight, ambLight,framebuffer->mFBO);

		//pass02 将colorAttachment渲染到屏幕上
		gRuntime.renderer->render(gRuntime.scene, gRuntime.camera, gRuntime.dirLight, gRuntime.pointLights, gRuntime.spotLight, gRuntime.ambLight);

		renderIMGUI(gDebug);
	}

	//4 退出程序前清理资源

	shutdown(gRuntime);
	appGI->destroy();

	return 0;
}
