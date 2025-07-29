#pragma once

#include "Window.h"
#include "Model.h"
#include "Camera.h"
#include "GameSystem.h"
#include "Light.h"
#include "GUI.h"
#include "ResourceManager.h"

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
	ModelManager modelManager;
	FontManager fontManager;
	EventManager eventManager;
	Camera camera;

	Light light;

	Cubemap* skybox;

	GameSystem gameSystem;
	GUI gui;
};