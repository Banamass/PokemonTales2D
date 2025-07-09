#pragma once

#include <math.h>

#include "SharedTypes.h"
#include "Window.h"
#include "Model.h"

class Box {
public:
	Box(glm::vec2 l_pos, Model* model, Shader* shader);
	~Box();

	void Draw(Window* win);

	void Select();
	void Unselect();

	void SetColor(glm::vec3 color);
	
	Transform* GetTransform() { return sprite.GetTransform(); }

private:
	Drawable sprite;

	glm::vec3 unselectedColor;
	glm::vec3 selectedColor;

	glm::vec2 pos;
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
	Model boxModel;
	Shader* boxShader;
	SharedContext* context;

	glm::ivec2 size;

	Box* selectedBox;

	std::vector<std::vector<Box>> boxes;
	Drawable* boxDrawable;
	std::vector<Transform*> transforms;
	DrawableInstanced* boxesDrawable;
};