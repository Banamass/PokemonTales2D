#pragma once

#include "Shader.h"
#include "Window.h"

class Model {
public:

private:

};

class Object {
public:
	Object();
	~Object();
	
	void SetShader(Shader* l_shader);

	void Draw(const glm::mat4& cameraMatrix);

private:
	Shader* shader;

	unsigned int VBO;
	unsigned int VAO;

	glm::mat4 modelMatrix;
};