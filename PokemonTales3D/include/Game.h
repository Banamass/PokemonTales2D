#pragma once

#include "Window.h"
#include "Model.h"
#include "Camera.h"

class Game {
public:
	Game();
	~Game();

	void Update(double dt);
	void Render();

	bool IsOver();

private:
	SharedContext context;

	Window window;
	EventManager eventManager;
	Camera camera;

	Shader shader;
	Shader modelShader;
	Object model;
	Model backpack;
};