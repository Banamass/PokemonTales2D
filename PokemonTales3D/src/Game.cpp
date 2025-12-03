#include "Game.h"

Game::Game()
	: window(Constants::WIN_WIDTH, Constants::WIN_HEIGHT, "PokeTales3D", &context),
	eventManager(&context),
	shaderManager(&context), 
	modelManager(&context), 
	fontManager(&context),
	stateManager(&context)
{
	srand(time(0));

	context.game = this;
	context.gameData = &dataManager;

	window.SetCursorCapture(true);

	stateManager.AddState(StateType::Battle, new BattleState(&context));
	stateManager.AddState(StateType::Menu, new MenuState(&context));
	stateManager.AddState(StateType::OpenWorld, new OpenWorldState(&context));
	stateManager.AddState(StateType::Options, new OptionsState(&context));

	SwitchState(StateType::Menu);
}
Game::~Game(){
	
}

void Game::Update(double dt){
	stateManager.Update(dt);
}
void Game::Render(){
	window.StartDraw();

	stateManager.Render();

	window.EndDraw();
}

bool Game::IsOver() {
	return window.GetShouldClose();
}

void Game::SwitchState(StateType type) {
	stateManager.ActivateState(type);
	eventManager.SetState(type);
}