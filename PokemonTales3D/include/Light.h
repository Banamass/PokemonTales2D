#pragma once

#include "Window.h"

class Light {
public:
	Light(glm::vec3 pos, SharedContext* l_context);
	~Light();

	void Draw(Window* win);

private:
	Model model;
	Shader* shader;
	Drawable sprite;
};