#include "GameSystem.h"

GameSystem::GameSystem(SharedContext* l_context) : board(l_context), context(l_context) {
	context->board = &board;
}
GameSystem::~GameSystem() {

}

void GameSystem::Update() {
	board.Update();
}
void GameSystem::Render() {
	board.Render();
}