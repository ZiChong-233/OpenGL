#pragma once

#include<iostream>
//#include <GLFW/glfw3.h>此处包含的话，在main.cpp中会冲突，应避免

#define appGI Application::getInstance()

//目标：封装application

class GLFWwindow;//给下面的mWindow指针用的前置声明 **？暂时不理解它和#include <GLFW/glfw3.h>的区别

using ResizeCallback = void(*)(int width, int height);
using KeyBoardCallback = void(*)(int key, int action, int mods);//为什么不要scancode？暂时不理解
using MouseCallback = void(*)(int button, int action, int mods);
using CursorCallback = void(*)(double xpos, double ypos);
using ScrollCallback = void(*)(double offset);

class Application {
public:
	~Application();

	bool init(const int& width = 800, const int& height = 600);
	bool update();
	void destroy();

	GLFWwindow* getWindow() { return mWindow; }

	//用于访问实例的静态函数
	static Application* getInstance();

	uint32_t getWidth() { return mWidth; }
	uint32_t getHeight() { return mHeight; }

	void getCursorPosition(double* x, double* y);

	void setResizeCallback(ResizeCallback callback) { mResizeCallback = callback; }
	void setKeyBoardCallback(KeyBoardCallback callback) { mKeyBoardCallback = callback; }
	void setMouseCallback(MouseCallback callback) { mMouseCallback = callback; }
	void setCursorCallback(CursorCallback callback) { mCursorCallback = callback; }
	void setScrollCallback(ScrollCallback callback) { mScrollCallback = callback; }

private:
	static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	static void keyCallbck(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow* window, int button , int action, int mods);
	static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	//全局唯一的静态变量实例
	static Application* minstance;

	uint32_t mWidth = { 0 };
	uint32_t mHeight = { 0 };
	GLFWwindow* mWindow = nullptr;

	ResizeCallback mResizeCallback = nullptr;
	KeyBoardCallback mKeyBoardCallback = nullptr;
	MouseCallback mMouseCallback = nullptr;
	CursorCallback mCursorCallback = nullptr;
	ScrollCallback mScrollCallback = nullptr;

	Application();
};