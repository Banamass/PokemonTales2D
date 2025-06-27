#include "Game.h"

Game::Game() 
	: window(800, 600, "PokeTales3D"), eventManager(&window)
	,shader("shaders\\SimpleVertexShader.glsl", "shaders\\SimpleFragmentShader.glsl"),
	camera(&eventManager, &window){
	context.win = &window;
	context.eventManager = &eventManager;
	model.SetShader(&shader);

	window.SetCursorCapture(true);
}
Game::~Game(){}

void Game::Update(double dt){
	camera.Update(dt);
}
void Game::Render(){
	window.StartDraw();

	model.Draw(camera.GetTransformMatrix());

	window.EndDraw();
}

bool Game::IsOver() {
	return window.GetShouldClose();
}