#pragma once

#include "Window.h"
#include "Model.h"
#include "Camera.h"
#include "GameSystem.h"
#include "Light.h"
#include "GUI.h"

class Game {
public:
	Game();
	~Game();

	void Update(double dt);
	void Render();

	bool IsOver();

private:

	void InitFont();

	SharedContext context;

	Window window;
	ShaderManager shaderManager;
	EventManager eventManager;
	Camera camera;

	Light light;

	Cubemap* skybox;

	GameSystem gameSystem;
	GUI gui;
};