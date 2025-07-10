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

	SharedContext context;

	Window window;
	ShaderManager shaderManager;
	EventManager eventManager;
	Font font;
	Camera camera;

	Light light;

	Cubemap* skybox;

	Text text;

	GameSystem gameSystem;
	GUI gui;
};