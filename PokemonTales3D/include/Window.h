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
#include "Model.h"

class Window;

class Drawable {
public:
	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float shininess;
	};

	Drawable(Model* model, Shader* shader);
	Drawable(Model* model, Shader* shader, Material l_material);
	~Drawable();

	void Move(glm::vec3 move);
	/*Warning : have to be call first after the creation of the Drawable Object*/
	void Scale(glm::vec3 scale);
	void Rotate(glm::vec3 rotation);

	void SetMaterial(const Material& l_materiel);

private:
	void ResetTransformations();
	void ComputeTransform();

	Material material;

	Model* model;
	Shader* shader;
	glm::vec3 translation;
	glm::vec3 scaling;
	glm::vec3 rotation; //pitch(around x) yaw(around y) roll(around z)
	glm::mat4 transform;

	friend class Window;
};

class Window {
public:
	Window(int l_width, int l_height, const std::string& winName, SharedContext* context);
	~Window();

	void StartDraw();
	void EndDraw();
	void Draw(Drawable& drawable);

	void SetCursorCapture(bool capture);

	glm::vec2 GetWindowSize();
	bool GetShouldClose();
	GLFWwindow* GetGLFWwindow();

private:
	void ProcessInput();

	SharedContext* context;

	GLFWwindow* window;
	int width;
	int height;
};