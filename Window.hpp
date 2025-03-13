#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Camera.hpp"

struct DebugInformation {
	bool showWireframe = false;
	bool depthBuffer = false;
	bool stylized = false;
};

struct Window {
	Window();
	~Window();
	bool shouldClose() const;
	void runLoopFunctions();
	double getTime() const;

	static void errorCallback(int error, const char* description);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	void processInput();

	bool initialized = false;
	GLFWwindow* window;
	int height = 600; 
	int width = 600;
	Camera camera;
	double lastFrame = 0.0;
	double deltaTime = 0.0;

	DebugInformation debugInformation;
};