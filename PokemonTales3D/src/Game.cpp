#include "Game.h"

Game::Game() 
	: window(800, 600, "PokeTales3D", &context), eventManager(&context),
	camera(&context), shaderManager(&context),
	board(glm::vec2(4, 3), &context)
	,light(glm::vec3(10.0f, 3.0f, 10.0f), &context){
	window.SetCursorCapture(true);
}
Game::~Game(){}

void Game::Update(double dt){
	camera.Update(dt);
}
void Game::Render(){
	window.StartDraw();

	board.Draw();
	light.Draw(&window);

	window.EndDraw();
}

bool Game::IsOver() {
	return window.GetShouldClose();
}