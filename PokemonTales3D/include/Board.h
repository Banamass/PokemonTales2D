#pragma once

#include <math.h>

#include "SharedTypes.h"
#include "Window.h"
#include "Model.h"
#include "APlayer.h"

class Box {
public:
	Box(glm::ivec2 l_pos);
	~Box();

	void Select();
	void Unselect();

	glm::vec2 GetPos() { return pos; }

private:
	glm::vec3 selectedColor;

	glm::ivec2 pos;
	float size;
};

class Board {
public:
	Board(glm::vec2 l_size, SharedContext* context);
	~Board();

	void Update(double dt);
	void Draw();

	bool Contain(const glm::ivec2& pos);
	Box* GetBox(const glm::ivec2& pos);

private:
	SharedContext* context;

	Model boxModel;
	Shader* instancedModelShader;
	Shader* modelShader;

	glm::ivec2 size;

	std::vector<std::vector<Box>> boxes;
	std::vector<Transform*> boxesTransforms;
	Drawable* boxDrawable;
	DrawableInstanced* boxesDrawable;

	Drawable* cursorDrawable;
	SquareArea cursor;
};