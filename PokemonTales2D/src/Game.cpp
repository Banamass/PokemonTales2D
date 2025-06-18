#include "Game.h"

Game::Game() : window("PokeTales2D", sf::Vector2u(1920/1.5, 1080/1.5), &context), gameSystem(&context) {
	frametime = 1.0f / 60.0f;

	context.window = &window;
}
Game::~Game(){}

void Game::Update(){
	while (elapsed.asSeconds() >= frametime) {
		window.Update();
		gameSystem.Update();
		elapsed -= sf::seconds(frametime);
	}
}
void Game::LateUpdate(){
	RestartClock();
}
void Game::Render(){
	window.BeginDraw();
	
	gameSystem.Render();

	window.EndDraw();
}

Window* Game::GetWindow() { return &window; }
sf::Time Game::GetElapsed() { return elapsed; }
void Game::RestartClock() { elapsed += clock.restart(); }