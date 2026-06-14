#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>//每一个cpp文件单独编译，不需要担心头文件重复包含的问题

//初始化Application静态变量
Application* Application::minstance = nullptr;
Application* Application::getInstance() {
	//如果实例化了，就直接返回
	//如果没有实例化（new），就先实例化
	if (minstance == nullptr) {
		minstance = new Application();
	}
	return minstance;
}	
//**一开始忘记在这里定义构造函数和析构函数，导致链接错误
Application::Application() {
	std::cout << "Application 构造函数" << std::endl;
}
Application::~Application() {
	std::cout << "Application 析构函数" << std::endl;
}

bool Application::init(const int& width, const int& height){
	mWidth = width;
	mHeight = height;
	//1 初始化GLFW基本环境
	if (!glfwInit()) {
		std::cout << "GLFW初始化失败" << std::endl;
		return -1;
	}
	//1.1设置GLFW版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//1.2设置OpenGL启用核心模式（非立即渲染模式）
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//2 创建窗体对象
	mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGLStudy", NULL, NULL);
	if (mWindow == NULL) {
		return false;
	}
	//**设置当前窗体对象为OpenGL的绘制舞台
	glfwMakeContextCurrent(mWindow);

	//****使用glad加载所有当前版本OpenGL的函数
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD!!!" << std::endl;
		return false;
	}
	std::cout << "OpenGL版本号：" << glGetString(GL_VERSION) << std::endl;//打印OpenGL版本号（额外补充

	glfwSetFramebufferSizeCallback(mWindow, frameBufferSizeCallback);

	glfwSetWindowUserPointer(mWindow,this);
	//this就是当前全局唯一的Application对象，把this存到mWindow

	//键盘响应
	glfwSetKeyCallback(mWindow, keyCallbck);

	//鼠标点击事件响应
	glfwSetMouseButtonCallback(mWindow, mouseCallback);

	//鼠标移动事件响应
	glfwSetCursorPosCallback(mWindow, cursorCallback);

	//鼠标滚轮事件响应
	glfwSetScrollCallback(mWindow, scrollCallback);

	return true;
}
bool Application::update(){
	if(glfwWindowShouldClose(mWindow)){
		return false;
	}
	//接收并分发窗体消息
	//检查消息队列是否有需要处理的鼠标、键盘等消息
	//若有责将消息批量处理，清空队列
	glfwPollEvents();

	//切换双缓存
	glfwSwapBuffers(mWindow);

	return true;
}
void Application::destroy(){
	glfwTerminate();
}

void Application::getCursorPosition(double* x , double* y) {
	glfwGetCursorPos(mWindow , x , y );
}

void Application::frameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	std::cout << "Resize" << std::endl;
	
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mResizeCallback != nullptr) {
		self->mResizeCallback(width, height);
	}//注意，必须进行判空


	//if(Application::getInstance()->mResizeCallback!=nullptr){
	//	Application::getInstance()->mResizeCallback(width, height);
	//}
}

void Application::keyCallbck(GLFWwindow* window, int key, int scancode, int action, int mods) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mKeyBoardCallback != nullptr) {
		self->mKeyBoardCallback(key, action, mods);
	}
}

void Application::mouseCallback(GLFWwindow* window, int button, int action, int mods) {//static和override都不需要重新指出
	//std::cout << "鼠标点击:" << button << "," << action<<std::endl;
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mMouseCallback != nullptr) {
		self->mMouseCallback(button, action, mods);
	}
}
void Application::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	/*std::cout << "鼠标移动:" << xpos << "," << ypos << std::endl;*/
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mCursorCallback != nullptr) {
		self->mCursorCallback(xpos, ypos);
	}
}
//滚动消息的xoffset没用
void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	Application* self = (Application*)glfwGetWindowUserPointer(window);
	if (self->mScrollCallback != nullptr) {
		self->mScrollCallback(yoffset);
	}
}