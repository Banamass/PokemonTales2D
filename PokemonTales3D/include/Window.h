#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SharedTypes.h"

class Window {
public:
	Window(int l_width, int l_height, const std::string& winName);
	~Window();

	void StartDraw();
	void EndDraw();

	void SetCursorCapture(bool capture);

	glm::vec2 GetWindowSize();
	bool GetShouldClose();
	GLFWwindow* GetGLFWwindow();

private:
	void ProcessInput();

	GLFWwindow* window;
	int width;
	int height;
};