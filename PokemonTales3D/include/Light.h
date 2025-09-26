#pragma once

#include "Window.h"

class Light {
public:
	Light(glm::vec3 pos, SharedContext* l_context);
	~Light();

	//Draw the light
	void Draw(Window* win);

private:
	SharedContext* context;

	Model* model;
	Shader* shader;
	Drawable* sprite;
};