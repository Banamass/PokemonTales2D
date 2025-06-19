#include "GameSystem.h"

GameSystem::GameSystem(SharedContext* l_context) : board(l_context), context(l_context), player(l_context) {
	context->board = &board;
	player.Setup();
}
GameSystem::~GameSystem() {

}

void GameSystem::Update() {
	player.Update();
	board.Update();
}
void GameSystem::Render() {
	board.Render();
	player.Render();
}