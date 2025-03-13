#include "Window.hpp"

void Window::errorCallback(int error, const char* description)
{
	std::cerr << "[GLFW error] " << description << std::endl;
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* w = (Window*)glfwGetWindowUserPointer(window);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
		w->debugInformation.showWireframe = !w->debugInformation.showWireframe;

	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
		w->debugInformation.depthBuffer = !w->debugInformation.depthBuffer;

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		w->debugInformation.stylized = !w->debugInformation.stylized;
}

void Window::processInput() 
{
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.roll(-deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.roll(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.pitch(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.pitch(-deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.yaw(deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.yaw(-deltaTime);
	}
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.forward(deltaTime * 500);
	}
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.backward(deltaTime * 500);
	}
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		camera.left(deltaTime * 500);
	}
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		camera.right(deltaTime * 500);
	}
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Window* w = (Window*)glfwGetWindowUserPointer(window);
	glViewport(0, 0, width, height);
	w->height = height;
	w->width = width;
}

Window::Window()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW initialization failed." << std::endl;
	}
	glfwSetErrorCallback(errorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(height, width, "Terrain", NULL, NULL);
	if (!window)
	{
		std::cerr << "Window creation failed." << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "GLAD initialization failed." << std::endl;
		glfwTerminate();
	}

	glfwSetKeyCallback(window, keyCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetWindowUserPointer(window, this);
	glfwSwapInterval(1);

	initialized = true;
}

Window::~Window()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(window);
}

void Window::runLoopFunctions()
{
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	glfwSwapBuffers(window);
	glfwPollEvents();
	processInput();
}

double Window::getTime() const
{
	return glfwGetTime();
}
