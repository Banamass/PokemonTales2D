#pragma once

#include "SharedTypes.h"
#include "Window.h"
#include "Model.h"

class Box {
public:
	Box(glm::vec2 l_pos, Model* model, Shader* shader);
	~Box();

	void Draw(Window* win);

private:
	Drawable sprite;

	glm::vec2 pos;
	float size;
};

class Board {
public:
	Board(glm::vec2 l_size, SharedContext* context);
	~Board();

	void Update(double dt);
	void Draw();

private:
	Model boxModel;
	Shader* boxShader;
	SharedContext* context;

	glm::vec2 size;

	std::vector<Box> boxes;
};