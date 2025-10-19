#pragma once

#include "Window.h"
#include "Model.h"
#include "Camera.h"
#include "GameSystem.h"
#include "Light.h"
#include "GUI.h"
#include "ResourceManager.h"
#include "BattleState.h"
#include "MenuState.h"
#include "OptionsState.h"

//The main class of the game
class Game {
public:
	Game();
	~Game();

	void Update(double dt);
	void Render();

	bool IsOver();
	void SwitchState(StateType type);

private:
	SharedContext context;

	Window window;
	ShaderManager shaderManager;
	ModelManager modelManager;
	FontManager fontManager;
	EventManager eventManager;
	StateManager stateManager;
	DataManager dataManager;
};