#pragma once

#include "Window.h"

class Light {
public:
	Light(glm::vec3 l_pos, SharedContext* l_context);
	~Light();

	//Draw the light
	void Draw();

	//Activate the light in current shaders
	void Activate();
	//Desactivate the light in current shaders
	void Desactivate();

private:
	SharedContext* context;

	glm::vec3 pos;

	Model* model;
	Shader* shader;
	Drawable* sprite;
};