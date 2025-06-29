#include "Window.h"

Window::Window(int l_width, int l_height, const std::string& winName)
	: width(l_width), height(l_height){
	glewExperimental = true;

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(800, 600, winName.c_str(), NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	glViewport(0, 0, width, height);

	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return;
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
}
Window::~Window() {
	glfwTerminate();
}

void Window::ProcessInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void Window::StartDraw() {
	ProcessInput();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Window::EndDraw() {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Window::SetCursorCapture(bool capture) {
	if (capture) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

bool Window::GetShouldClose() {
	return glfwWindowShouldClose(window);
}

glm::vec2 Window::GetWindowSize() {
	return glm::vec2(width, height);
}
GLFWwindow* Window::GetGLFWwindow() {
	return window;
}
